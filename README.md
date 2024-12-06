# CN_CHomeworks_3
this project contain some Classes : DatalinkHeader , TcpHeader , MacAddressGenerator , Packet , Datagenerator , EventsCoordinator , TcpHeader and other Classes . 
explain codes of DataGenerator Class

The DataGenerator class is designed to simulate the generation of random data based on a pareto distribution. It provides methods to generate individual data points as well as a series of data points over a specified time duration.
features of this Class:
Constructor

Purpose: Initializes the generator with distribution parameters and a random seed.
Parameters:
        shape: Controls the shape of the distribution.
        scale: A scaling factor for the generated values.
        rate: Specifies the number of data points generated per second.
        seed: An unsigned integer to seed the random number generator, ensuring reproducibility.

generate Method
Purpose: Produces a single data value based on a custom distribution formula.
Working:
Draws a uniform random number u in the range [0, 1).
uses the formula scale / std::pow(1 - u, 1.0 / shape) to calculate the output.
generateOverTime Method

Purpose: Generates multiple data points over a specified time period.
Parameters:
time_in_seconds: The duration (in seconds) over which data will be generated.
Working:
Calculates the number of samples to generate using the formula rate * time_in_seconds.
Calls generate() repeatedly to populate a vector with the required number of samples.
Return: A vector of generated data points.

explain code MacAddressGenerator :
The GenerateMACAddress class provides functionality to generate both random and unique MAC (Media Access Control) addresses.

A random number generator (std::mt19937) is seeded using std::random_device.
The std::uniform_int_distribution<int> is used to generate random integers in the range [0, 255].

The octet is formatted as a 2-digit hexadecimal value and added to the stream.
Colons (:) are inserted between octets, except after the last one.

generateUniqueMAC Method
Purpose: Ensures that the generated MAC address is unique during the program's execution.
    Logic:
        Calls generateRandomMAC to create a random MAC address.
        Checks if the generated address already exists in the generatedMACs set.
        If the address is unique, it is added to the set and returned.

Key Features of the Code
 Random Generation:The use of std::random_device and std::mt19937 ensures high-quality random numbers for generating MAC addresses.
Locally Administered Addresses:The first octet is modified to conform to the standard MAC address specification:
Locally Administered Bit (second-least significant bit) is set.
Multicast Bit (least significant bit) is cleared.

Uniqueness:

    The generateUniqueMAC method maintains a record of all previously generated MAC addresses to avoid duplication.

Purpose: Generates a random MAC address.
Logic:
        A MAC address consists of 6 octets (each 8 bits, or one byte), separated by colons (:).
        Each octet is a random value between 0 and 255 (or 0x00 to 0xFF in hexadecimal).
        The first octet is modified to ensure the address is locally administered and not multicast.
        
macStream is used to build the MAC address string in the desired format.

explain of Packet Class :
The Packet class represents a network packet, encapsulating its headers, payload, and other metadata. This implementation focuses on managing the packet's lifecycle, including dynamic memory allocation and providing accessors for its data.

Initialization:
    Assigns all provided parameters to the corresponding member variables.
    Sets default values for omitted parameters for construct object of this class

deconstructor part:
Purpose: Cleans up dynamically allocated memory to avoid memory leaks.
Behavior:
Deletes tcpHeader and dataLinkHeader, which were allocated dynamically.

Safety: Ensure that delete is only called on dynamically allocated objects to prevent undefined behavior.

getter functions:

These methods return values or pointers to the class's members, allowing controlled read access.
The const qualifier ensures that the returned references or pointers cannot modify the original member variables.

setter functions:
setPath: Sets the entire path of the packet (a list of hops).
addHop: Appends a single hop (e.g., a node or device) to the path.


These methods return values or pointers to the class's members, allowing controlled read access.
The const qualifier ensures that the returned references or pointers cannot modify the original member variables.

Class Responsibilities

The Packet class is responsible for:
Encapsulation: Encapsulates data related to a network packet, such as headers, payload, and metadata.
Memory Management: Ensures proper handling of dynamically allocated resources.
Flexibility: Provides accessors (getters and setters) to interact with its data in a controlled manner.
Path Tracking: Tracks the path taken by the packet as a series of hops.

Explanation of the Packet Class Implementation

The Packet class represents a network packet, encapsulating its headers, payload, and other metadata. This implementation focuses on managing the packet's lifecycle, including dynamic memory allocation and providing accessors for its data.
Detailed Code Explanation

   
Purpose of constructor: Initializes a Packet instance with the provided parameters.

Parameters:
            UT::PacketType type: Defines the type of the packet (e.g., Data or Control).
            TcpHeader *tcp: A pointer to the TCP header associated with this packet.
            DataLinkHeader *dataLink: A pointer to the data link layer header.
            const string &dataPayload: The payload (data) carried by the packet.
            int seqNum: The sequence number of the packet.
            int waitCycles: The number of cycles the packet waits before transmission.
            int total: The total number of cycles for packet processing.

Initialization:
        Assigns all provided parameters to the corresponding member variables.
        Sets default values for omitted parameters (e.g., bgpHeader is not used here).


Purpose of deconstructor: Cleans up dynamically allocated memory to avoid memory leaks.
        Behavior:
            Deletes tcpHeader and dataLinkHeader, which were allocated dynamically.
            ipHeader and bgpHeader are commented out, indicating they are not used in this configuration.
        Safety: Ensure that delete is only called on dynamically allocated objects to prevent undefined behavior.

Getters
purpose: Provide access to the private member variables of the class.


These methods return values or pointers to the class's members, allowing controlled read access.
The const qualifier ensures that the returned references or pointers cannot modify the original member variables.

Setters
Purpose: Allow modification of specific member variables.

setPath: Sets the entire path of the packet (a list of hops).
addHop: Appends a single hop (e.g., a node or device) to the path.


        


Class Responsibilities

The Packet class is responsible for:

    Encapsulation: Encapsulates data related to a network packet, such as headers, payload, and metadata.
    Memory Management: Ensures proper handling of dynamically allocated resources.
    Flexibility: Provides accessors (getters and setters) to interact with its data in a controlled manner.
    Path Tracking: Tracks the path taken by the packet as a series of hops.

Key Member Variables

UT::PacketType packetType: Defines the type of packet (e.g., Data or Control).
TcpHeader *tcpHeader: A pointer to the TCP header for transport-layer information.
DataLinkHeader *dataLinkHeader: A pointer to the data link header for physical-layer information.
std::string payload: The actual data carried by the packet.
int sequenceNumber: The packet's sequence number, typically used for ordering in a stream.
int waitingCycles: Number of cycles before the packet is transmitted.
int totalCycles: Total processing cycles required for the packet.
std::vector<std::string> path: Tracks the nodes or hops the packet has traversed.

explain of main.cpp codes

goal of parsing json file :
Purpose: Load and parse a configuration file in JSON format (config.js).

Steps:

Open the file and read its content.
Parse the JSON into a QJsonDocument.
Extract simulation parameters, such as duration, routing protocol, and topology.

Key Parameters:

simulation_duration, cycle_duration: Define time-related settings.
TTL: Time-to-live for packets.
packets_per_simulation: Number of packets generated.
router_buffer_size, router_port_count: Router-related settings.

Purpose: Parse details about autonomous systems (AS) in the network.
Details Extracted:

AS ID (id), topology type (topology_type), node count (node_count).
Gateway configurations (as_gateways and user_gateways).
Connections to other ASes and broken router details.

Example:

Gateway nodes and user connections are extracted for each AS.

key Components of the Simulation

JSON Configuration: Sets up the simulation environment based on user-defined parameters.
Data Generation: Simulates data traffic using statistical distributions.
Packet Management: Manages packet lifecycle, including headers and payloads.
Event Handling: Coordinates simulation events through signals and slots.
MAC Addressing: Ensures unique MAC addresses for network devices.

Advantages

Modular Design: Each functionality is encapsulated in a dedicated class.
Extensibility: Easily customizable by modifying JSON configurations or adding new components.
Realistic Simulation: Combines data generation, networking headers, and event handling for comprehensive simulation.

Example Output

Configuration parameters printed using qDebug.
Generated MAC addresses and data values printed to the console.
Packet details, including headers and path, logged.

explain of EventsCoordinator Class
EventsCoordinator is responsible for managing simulation events. This class acts as a coordinator and is responsible for scheduling the execution of various simulation components.

Functionality:

• Uses an internal timer to run the simulation at specified intervals (default is every second).
• Utilizes DataGenerator to generate data, which can be random numbers or specific values depending on the type of simulation.
• Sends signals such as tick (to announce each time tick) and dataGenerated (to send the generated data) to other components.
• The simulation can be controlled using the startSimulation, stopSimulation, and resetSimulation methods.

Importance:

EventsCoordinator acts as the central time management and simulation execution hub, coordinating various system components.

explain of TCPHeader Class
TCPHeader is a class that simulates the header of the Transmission Control Protocol (TCP). This class implements all the necessary and standard TCP fields.

Functionality:

• Includes the main TCP header fields:
• Source and destination ports to identify the origin and destination of the packet.
• Sequence number and acknowledgment number to manage packet order and receipt confirmation.
• Flags: control bits such as SYN, ACK, FIN, etc.
• Checksum: to ensure data integrity.
• Window Size: to control data flow.
• Supports serialization and deserialization:
• Serialization: converts the header to binary format for data transmission.
• Deserialization: reconstructs the header from received data.

Importance:

TCPHeader is a vital part of simulating the network transport layer. This class enables precise examination of packet transmission and TCP connection management.

explain of DataLinkHeader CLass
DataLinkHeader represents the header of the Data Link Layer and manages addressing information in this layer.

Functionality:

• Includes two main fields:
• Source MAC address: the hardware address of the origin.
• Destination MAC address: the hardware address of the destination.
• Allows setting and retrieving MAC addresses.

Importance:

DataLinkHeader is essential for simulating Layer 2 network. This class helps manage data routing in local area networks (LANs) and plays a key role in lower-layer communications.

Interrelation of Components

• EventsCoordinator: Manages events and overall simulation coordination.
• TCPHeader: Manages TCP packet headers in the transport layer.
• DataLinkHeader: Stores and manages addressing information in the Data Link Layer.

These three classes together form a network simulation system that is expandable and flexible.

Conclusion

The design of these three classes is such that they maintain simplicity while offering high flexibility. EventsCoordinator acts as the central coordinator, while TCPHeader and DataLinkHeader manage data packet information in different layers. This structure enables accurate and reliable network simulation.

class of SimulationHandler :
Key Features

    Event-Driven: Relies on Qt's signal-slot mechanism for handling simulation events.
    Modularity: Encapsulates simulation event handling, making it easy to modify or extend.
    Extensibility: Can be customized to perform more sophisticated actions, such as:
        Collecting and analyzing data.
        Interfacing with external systems.

Example Usage in a Simulation

    Simulation Start: Logs the start event and initializes required states.
    Periodic Tick: Updates internal logic or performs periodic tasks.
    Data Generation: Captures and processes data values as they are generated.
    Simulation End: Cleans up and finalizes the simulation process.
