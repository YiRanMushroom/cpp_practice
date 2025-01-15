module;

#include <iostream>
#include <boost/asio.hpp>


export module boost_asio;

using boost::asio::ip::tcp;

export void fetch_content(const std::string& server, const std::string& path) {
    try {
        boost::asio::io_context io_context;

        // Resolve the server address
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(server, "http");

        // Create and connect the socket
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Form the request
        std::string request = "GET " + path + " HTTP/1.1\r\n";
        request += "Host: " + server + "\r\n";
        request += "Connection: close\r\n\r\n";

        // Send the request
        boost::asio::write(socket, boost::asio::buffer(request));

        // Read the response
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        // Check the response status
        std::istream response_stream(&response);
        std::string http_version;
        unsigned int status_code;
        std::string status_message;
        response_stream >> http_version >> status_code;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cout << "Invalid response\n";
            return;
        }
        if (status_code != 200) {
            std::cout << "Response returned with status code " << status_code << "\n";
            return;
        }

        // Read the response headers
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Print the response headers
        std::string header;
        while (std::getline(response_stream, header) && header != "\r") {
            std::cout << header << "\n";
        }
        std::cout << "\n";

        // Read the response body
        boost::system::error_code error;
        while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
            std::cout << &response;
        }
        if (error != boost::asio::error::eof) {
            throw boost::system::system_error(error);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}