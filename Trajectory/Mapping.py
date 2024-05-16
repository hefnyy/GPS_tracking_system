import serial

def receive_from_com(port, baud_rate):  #Maryam
    try:
        # Open serial port
        ser = serial.Serial(port, baud_rate)

        while True:
            # Read a line from serial port
            received_string = ser.readline().decode('utf-8').strip()
            
            # Print the received string
            print("Received:", received_string)
            lat,lon = string_to_arrays(received_string)
            print(lat)
            print("\n lon: \n")
            print(lon)
            display_locations_on_map(lat, lon)
    except serial.SerialException as e:
        print("Serial Exception:", e)
    except KeyboardInterrupt:
        print("Keyboard Interrupt. Exiting...")
    finally:
        if ser and ser.is_open:
            ser.close()

def send_letter_to_com4(letter): #Maryam
    try:
        # Open the serial port
        ser = serial.Serial('COM4', 9600, timeout=1)
        
        # Ensure the serial port is open
        if ser.is_open:
            print(f"Serial port {ser.name} opened successfully.")
            
            # Send the letter
            ser.write(letter.encode())
            print(f"Sent letter '{letter}' to {ser.name}")
            
            # Close the serial port
            ser.close()
            print(f"Serial port {ser.name} closed.")
        else:
            print(f"Failed to open serial port {ser.name}")
    
    except serial.SerialException as e:
        print(f"Error: {e}")

