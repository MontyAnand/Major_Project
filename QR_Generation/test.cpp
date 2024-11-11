#include "qrcodegen.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace qrcodegen;

// Function to encode data in base64
string base64Encode(const vector<uint8_t>& data) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string encoded;
    int val = 0, valb = -6;
    for (uint8_t c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) encoded.push_back(table[((val << 8) >> (valb + 8)) & 0x3F]);
    while (encoded.size() % 4) encoded.push_back('=');
    return encoded;
}

// Function to generate BMP header
vector<uint8_t> createBmpHeader(int width, int height) {
    int rowSize = (width + 7) / 8; // row size for monochrome BMP
    int dataSize = rowSize * height;
    vector<uint8_t> header(62, 0); // BMP header size for 1-bit BMP

    // BMP Header
    header[0] = 'B';
    header[1] = 'M';
    int fileSize = dataSize + header.size();
    header[2] = fileSize & 0xFF;
    header[3] = (fileSize >> 8) & 0xFF;
    header[4] = (fileSize >> 16) & 0xFF;
    header[5] = (fileSize >> 24) & 0xFF;
    header[10] = 62; // offset to pixel array

    // DIB Header
    header[14] = 40; // DIB header size
    header[18] = width & 0xFF;
    header[19] = (width >> 8) & 0xFF;
    header[22] = height & 0xFF;
    header[23] = (height >> 8) & 0xFF;
    header[26] = 1; // color planes
    header[28] = 1; // bits per pixel (1-bit for monochrome)
    header[30] = 0; // BI_RGB, no compression

    // Color Table (black and white)
    header[54] = 0;    // Black
    header[55] = 0;
    header[56] = 0;
    header[57] = 0;
    header[58] = 255;  // White
    header[59] = 255;
    header[60] = 255;
    header[61] = 0;

    return header;
}

int main() {
    // Generate QR code
    const string text = "https://example.com";
    const QrCode qr = QrCode::encodeText(text.c_str(), QrCode::Ecc::LOW);
    int size = qr.getSize();
    int scale = 10;

    // Create BMP data
    int bmpWidth = size * scale;
    int bmpHeight = size * scale;
    vector<uint8_t> bmpHeader = createBmpHeader(bmpWidth, bmpHeight);
    vector<uint8_t> bmpData(bmpHeight * ((bmpWidth + 7) / 8), 0xFF); // White background

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (qr.getModule(x, y)) {
                for (int dy = 0; dy < scale; ++dy) {
                    for (int dx = 0; dx < scale; ++dx) {
                        int px = x * scale + dx;
                        int py = (size * scale - 1 - y * scale) - dy;
                        bmpData[py * ((bmpWidth + 7) / 8) + (px / 8)] &= ~(1 << (7 - (px % 8)));
                    }
                }
            }
        }
    }

    // Combine header and data, and encode to base64
    vector<uint8_t> bmp(bmpHeader);
    bmp.insert(bmp.end(), bmpData.begin(), bmpData.end());
    string base64Data = base64Encode(bmp);

    // Print base64 data
    cout<<base64Data.length()<<endl;
    cout << "data:image/bmp;base64," << base64Data << endl;

    return 0;
}

