#ifndef DATALINKHEADER_H
#define DATALINKHEADER_H
#include <string>

class DataLinkHeader {
public:
    DataLinkHeader();  // Constructor
    ~DataLinkHeader(); // Destructor

    void setSourceMAC(const std::string& mac);
    void setDestinationMAC(const std::string& mac);
    std::string getSourceMAC() const;
    std::string getDestinationMAC() const;

private:
    std::string sourceMAC;
    std::string destinationMAC;
};

#endif // DATALINKHEADER_H
