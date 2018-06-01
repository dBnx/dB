

#include "System.hpp"

#ifndef ___DB_SFML_NETWORK_HPP_		// Guard
#define ___DB_SFML_NETWORK_HPP_	

  #ifdef STATIC						// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-network-s-d.lib")
    #else
      #pragma comment (lib, "sfml-network-s.lib")
    #endif // _DEBUG

  #else								// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-network-d.lib")
    #else
      #pragma comment (lib, "sfml-network.lib")
    #endif // _DEBUG
  #endif  // STATIC

  #ifdef _MSC_VER 
    #pragma comment (lib, "Ws2_32.lib")
  #endif // _WINDOWS



#include <SFML\Network.hpp>
#include <string>
#include <string_view>
#include <sstream>
#include <optional>

namespace dB::network
{
	const std::optional<std::string> GetFileName(const std::string url, const std::string seperator = "/")
	{
		const size_t offset = url.rfind(seperator) + 1;
		return (offset > 0 && offset < url.size()) 
			? url.substr(offset, url.size() - offset) 
			: std::optional<std::string>{};
	} 

	// Remove the http(s):// 
	std::string RemoveProtocol(const std::string& host)
	{
		const char * format = "://";
		const size_t pos = host.find(format) + std::strlen(format);

		if (pos != std::string::npos)
			return host.substr(pos, host.size() - pos);
		else
			return host;
	}

	class URL
	{
	private:
		std::string host = "";
		std::string path = "";

		const std::string _Path_AddFirstSlash(const std::string host)
		{
			if (host.at(0) != '/')
			{
				return host;
			}
			std::string ret("/");
			return ret.append(host);
		}

		void _Set(const std::string_view url)
		{
			const std::string str = RemoveProtocol(std::string(url));

			const size_t pos = str.find('/');
			if (pos != std::string::npos)
			{
				this->host = str.substr(0, pos);
				this->path = str.substr(pos, str.size() - pos);
			}
			else 
			{
				this->host = std::move(str);
				this->path = "/";
			}
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
			this->host = RemoveProtocol(host);
 		}

		void Path(const std::string path)
		{
			this->path = this->_Path_AddFirstSlash(path);
		}

		void Set(const std::string_view url)
		{
			this->_Set(url);
		}

		URL()
		{

		}

		URL(const std::string_view url)
		{
			this->_Set(url);
		}

		URL(const std::string& host, const std::string& path) : path(path)
		{
			this->host = RemoveProtocol(host);
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

	constexpr bool IsResponseSuccesfull(int statusCode )
	{
		return statusCode - 200 < 100;
	}
}

#endif ___DB_SFML_NETWORK_HPP_		// Guard