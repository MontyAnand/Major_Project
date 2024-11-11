package Models

// Protocol = 0
type PortRequest struct {
	Operation     uint8 // Allow = 1 or Block = 0
	Port          uint16
	IsDestination uint8 // source = 0, Destination = 1
}

// Protocol = 1
type IPAddressReuqest struct {
	Opration      uint8 // Block = 0, Allow = 1
	IP            uint32
	Netmask       uint8
	IsDestination uint8 // Source = 0, Destination = 1
}
