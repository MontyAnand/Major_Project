package Utility

import (
	"bytes"
	"encoding/binary"
	"errors"
	"reflect"
)

func SerializeStruct(buffer *bytes.Buffer, data interface{}) error {
	ref := reflect.ValueOf(data)
	if ref.Kind() != reflect.Struct {
		return errors.New("passed argument is not of type Struct")
	}
	for i := 0; i < ref.NumField(); i++ {
		field := ref.Field(i)
		switch field.Kind() {
		case reflect.Uint8:
			{
				if err := binary.Write(buffer, binary.LittleEndian, uint8(field.Uint())); err != nil {
					return err
				}
			}
		case reflect.Uint16:
			{
				if err := binary.Write(buffer, binary.LittleEndian, uint16(field.Uint())); err != nil {
					return err
				}
			}
		case reflect.Uint32:
			{
				if err := binary.Write(buffer, binary.LittleEndian, uint32(field.Uint())); err != nil {
					return err
				}
			}
		case reflect.String:
			{
				strBytes := []byte(field.String())
				if _, err := (*buffer).Write(strBytes); err != nil {
					return err
				}
			}
		}
	}
	return nil
}
