import serial
import time
import folium
import os

def receive_from_com(port, baud_rate): 
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

def send_letter_to_com4(letter):
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

def string_to_arrays(s):  
    # Split the string into individual float strings
    float_strings = s.split(',')
    
    # Convert the float strings back to floats
    float_values = [float(num) for num in float_strings if num]  # Ensure no empty strings are converted
    
    # Assuming the arrays should be split evenly, we determine the midpoint
    midpoint = len(float_values) // 2
    
    # Split the float values into two arrays
    latitudes = float_values[:midpoint]
    longitudes = float_values[midpoint:]
    
    # Remove zero elements from the arrays
    latitudes = [lat for lat in latitudes if lat != 0]
    longitudes = [lon for lon in longitudes if lon != 0]
    
    return latitudes, longitudes

def display_locations_on_map(latitudes, longitudes):  
    # Create a map object with OpenStreetMap basemap
    map_obj = folium.Map(location=[latitudes[0], longitudes[0]], zoom_start=15, tiles='OpenStreetMap')

    # Create a list of locations
    locations = list(zip(latitudes, longitudes))

    # Add markers for start and end locations with labels indicating the order
    folium.Marker(locations[0], tooltip="Start", icon=folium.Icon(color="blue", icon="map-marker")).add_to(map_obj)
    folium.Marker(locations[-1], tooltip="End", icon=folium.Icon(color="red", icon="map-marker")).add_to(map_obj)
    folium.PolyLine(locations, color="blue", weight=2.5, opacity=1).add_to(map_obj)  #lines connecting the path
  
    # Save and open the map to an HTML file
    html_file_path = "locations_map.html"
    map_obj.save(html_file_path)
    os.system(html_file_path)

def read_from_uart(): 
    # Set up the serial connection (modify parameters as per your setup)
    ser = serial.Serial('COM4', 9600, timeout=1)
    
    # Send the command 'u' to the UART
    ser.write(b'u')
    
    # Wait for data to be received
    while True:
        if ser.in_waiting > 0:
            received_string = ser.readline().decode('utf-8').strip()
            break
        time.sleep(0.1)  # Small sleep to prevent busy waiting
    
    # Close the serial connection
    ser.close()
    
    return received_string

def main():    
    
    send_letter_to_com4('U')
    received_data = receive_from_com('COM4', 9600)
    print(received_data)
