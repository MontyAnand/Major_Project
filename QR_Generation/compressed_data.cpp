#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include "qrcodegen.hpp"  // Ensure you include the correct path to the qrcodegen header

using namespace std;
using namespace qrcodegen;

// Base64 Encoding Function (standard implementation)
std::string base64_encode(const std::vector<uchar>& buffer) {
    static const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string base64;
    int val = 0, valb = -6;
    for (unsigned char c : buffer) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            base64.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        base64.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (base64.size() % 4) {
        base64.push_back('=');
    }
    return base64;
}

// Function to generate QR code, convert it to PNG, and encode it in Base64
std::string generateCompressedQRCodeBase64(const std::string& data) {
    // Generate the QR code matrix
    const QrCode qr = QrCode::encodeText(data.c_str(), QrCode::Ecc::LOW);

    // Convert QR code matrix to OpenCV image (BMP format)
    int size = qr.getSize();
    cv::Mat img(size, size, CV_8UC1, cv::Scalar(255));  // Create a white image

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qr.getModule(x, y)) {
                img.at<uchar>(y, x) = 0;  // Black pixel for QR code
            }
        }
    }

    // Encode image as PNG in memory (compress BMP to PNG)
    std::vector<uchar> compressedData;
    cv::imencode(".png", img, compressedData);

    // Convert PNG data to Base64
    return base64_encode(compressedData);
}

int main() {
    std::string data = "https://example.com";
    
    // Generate QR code as Base64 encoded PNG
    std::string base64EncodedPng = generateCompressedQRCodeBase64(data);
    
    // Print Base64 encoded PNG QR Code
    std::cout << "Base64 Encoded Compressed QR Code (PNG): " << base64EncodedPng << std::endl;
    
    return 0;
}

