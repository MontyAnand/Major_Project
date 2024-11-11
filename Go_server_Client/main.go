package main

import (
	"bufio"
	"bytes"
	"fmt"
	"net"

	"major_project.com/Models"
	"major_project.com/Utility"
)

func formPacket(protocol uint8) []byte {
	var buffer bytes.Buffer
	buffer.Write([]byte{protocol})
	switch protocol {
	case 0:
		{
			payload := Models.PortRequest{}
			fmt.Println("Enter the Option: \n0. Block\n1. Accept")
			fmt.Scan(&payload.Operation)
			fmt.Println("Enter the Port:")
			fmt.Scan(&payload.Port)
			fmt.Println("Enter : \n0. Source\n1.Destination")
			fmt.Scan(&payload.IsDestination)
			Utility.SerializeStruct(&buffer, payload)
			fmt.Println(buffer.Bytes())
			return buffer.Bytes()
		}
	case 1:
		{
			payload := Models.IPAddressReuqest{}
			fmt.Println("Enter: \n0. Block\n1. Accept")
			fmt.Scan(&payload.Opration)
			fmt.Println("Enter IP ADDRESS")
			fmt.Scan(&payload.IP)
			fmt.Println("Enter Netmask")
			fmt.Scan(&payload.Netmask)
			fmt.Println("Enter: \n0. Source\n1. Destination")
			fmt.Scan(&payload.IsDestination)
			Utility.SerializeStruct(&buffer, payload)
			return buffer.Bytes()
		}
	default:
		return nil
	}
}
func main() {
	// Connect to the server
	conn, err := net.Dial("tcp", "127.0.0.1:8080")
	if err != nil {
		fmt.Println("Error connecting to server:", err)
		return
	}
	defer conn.Close() // Ensure connection is closed when done

	var choice uint8
	fmt.Println("Enter the opearation: \n0. Port Update\n1. IP request")
	fmt.Scan(&choice)
	_, err = conn.Write(formPacket(choice))
	if err != nil {
		fmt.Println("Error sending message:", err)
		return
	}
	// Receive response from the server
	response, err := bufio.NewReader(conn).ReadString('\n')
	if err != nil {
		fmt.Println("Error receiving response:", err)
		return
	}
	fmt.Println("Response from server:", response)
}
