

#include "System.hpp"

#ifndef ___DB_SFML_NETWORK_HPP_		// Guard
#define ___DB_SFML_NETWORK_HPP_	

  #ifdef STATIC						// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-network-s-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-network-s.lib")
    #endif // _DEBUG

  #else								// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-network-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-network.lib")
    #endif // _DEBUG
  #endif  // STATIC



#include <SFML\Network.hpp>
#include <string>
#include <sstream>
#include <optional>

namespace dB
{
	const std::optional<std::string> GetFileName(const std::string url, const std::string seperator = "/")
	{
		const size_t offset = url.rfind(seperator) + 1;
		return (offset > 0 && offset < url.size()) 
			? url.substr(offset, url.size() - offset) 
			: std::optional<std::string>{};
	} 

	class URL
	{
	private:
		std::string host;
		std::string path;

		const std::string _Host_RemoveProtocol(const std::string host)
		{
			// Dusnt wurk:
			// > "www.focus.de"
			// ->       "us.de"
			// > "fraghero.com"
			// ->      "ro.com"

			// Remove the http(s):// 
			const char * format = "://";
			const size_t pos = host.find_first_of(format)
				+ (sizeof format) / (sizeof format[0]); // Add ARRAYSIZE(format)
			
			return host.substr(pos, host.size() - pos);
		}

		const std::string _Path_AddFirstSlash(const std::string host)
		{
			if (host.at(0) != '/')
			{
				return host;
			}
			std::string ret("/");
			return ret.append(host);
		}

		void _Set(const std::string url)
		{
			const std::string str = this->_Host_RemoveProtocol(url);

			const size_t pos = url.find_first_of('/');
			this->host = str.substr(0, pos);
			this->path = str.substr(pos, url.size() - pos);
			// First Slash is present - therefore no need to call _Path_AddFirstSlash()
		}

	public:

		const std::string Host() const
		{
			return this->host;
		}

		const std::string Path() const
		{
			return this->path;
		}
		
		void Host(const std::string host)
		{
			this->host = this->_Host_RemoveProtocol(host);
 		}

		void Path(const std::string path)
		{
			this->path = this->_Path_AddFirstSlash(path);
		}

		URL(const std::string& url = "")
		{
			this->_Set(url);
		}

		void Set(const std::string& url)
		{
			this->_Set(url);
		}

		URL(const std::string& host, const std::string& path) : path(path)
		{
			this->host = this->_Host_RemoveProtocol(host);
		}

		std::optional<std::string> Get_FileName()
		{
			return GetFileName(this->path);
		}
	};

	const std::string Format(const sf::Http::Response& response, const int SkipBody = 0)
	{
		std::stringstream sts;
		sts << "<Http::Response> \n"
			<< " Status              = " << response.getStatus() << "\n"
			<< " HTTP version        = " << response.getMajorHttpVersion() << "." << response.getMinorHttpVersion() << "\n"
			<< " Content-Type header = " << response.getField("Content-Type") << "\n";
		if (!SkipBody)
			sts << " Body                = " << "\n" << response.getBody() << "\n";
		sts	<< "</Http::Response> \n";
		return sts.str();
	}
}

#endif ___DB_SFML_NETWORK_HPP_		// Guard