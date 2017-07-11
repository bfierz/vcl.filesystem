/*
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2014-2016 Basil Fierz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "webdavclient.h"

// Neon
#include <ne_props.h>
#include <ne_uri.h>
#include <ne_auth.h>
#include <ne_basic.h>

namespace Vcl { namespace FileSystem { namespace Util
{
	namespace
	{
		/**
		 *	\brief Callback for ne_set_server_auth to set passwort and username of ssl connection
		 */
		int authenticationCallback(void* userdata, const char* realm, int attempts, char* username, char* password)
		{
			const auto& auth = *reinterpret_cast<std::vector<std::string>*>(userdata);
			strncpy(username, auth[0].c_str(), NE_ABUFSIZ);
			strncpy(password, auth[1].c_str(), NE_ABUFSIZ);

			return attempts;
		}

		/*!
		 *	\brief Callback of ne_simple_propfind, that fills a vector(userdata) with properties of files/directory
		 */
		void propfindCallback(void* userdata, const ne_uri* uri, const ne_prop_result_set* set)
		{
			auto& paths = *reinterpret_cast<std::vector<WebDavPath>*>(userdata);

			ne_propname props[] =
			{
				{ "DAV:", "resourcetype" },
				{ "DAV:", "getlastmodified" },
				{ "DAV:", "getcontenttype" }
			};

			std::string ressourceType = ne_propset_value(set, &props[0]) ? ne_propset_value(set, &props[0]) : std::string();
			std::string lastModified = ne_propset_value(set, &props[1]) ? ne_propset_value(set, &props[1]) : std::string();
			std::string contentType = ne_propset_value(set, &props[2]) ? ne_propset_value(set, &props[2]) : std::string();
			
			WebDavPath path(uri->host,
				uri->path,
				ressourceType,
				lastModified,
				contentType);

			paths.push_back(path);
		}
	}

	static const ne_propname fetchProps[] =
	{
		{ "DAV:", "resourcetype" }, 
		{ "DAV:", "getlastmodified" },
		{ "DAV:", "getcontenttype"},
		{ 0 }
	};

	WebDavClient::WebDavClient(const std::string url, const unsigned port, const std::string user, const std::string pw)
	{
		// Capture the current error
		const char* err = "";

		// Create a new socket
		ne_sock_init();

		// Open a new session
		_session = ne_session_create("http", url.c_str(), port);
		err = ne_get_error(_session);

		// Authenticate to the server
		std::vector<std::string> auth;
		auth.emplace_back(user);
		auth.emplace_back(pw);
		ne_set_server_auth(_session, authenticationCallback, auth.data());
		err = ne_get_error(_session);
	}

	WebDavClient::~WebDavClient()
	{
		ne_forget_auth(_session);
		ne_session_destroy(_session);
		ne_sock_exit();
	}

	std::vector<WebDavPath> WebDavClient::listDirectory(const std::string& uri)
	{
		const int depth = NE_DEPTH_ONE;
		std::vector<WebDavPath>* props = new std::vector<WebDavPath>;
		int res = ne_simple_propfind(_session, uri.c_str(), depth, fetchProps, propfindCallback, props);
		if (res != NE_OK) {
			auto err = ne_get_error(_session);
			return *props;
		}
		props->erase(props->begin());
		return *props;
	}

#if 0

/**
 * @brief Checks for existance of file/diretory at
 *        given uri. This can be used to reduce transfer 
 *        bandwidth, so no data need to be transfered 
 *        which allready exists.
 *
 * @param uri of file/directory on webdav-server
 *
 * @return true if file/directory exists
 *         false if file/directory not exists
 *
 **/
bool WebDavClient::exist(std::string uri){
  const int depth = NE_DEPTH_ZERO; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
  std::vector<WebdavPath> * props = new std::vector<WebdavPath>;
  int res = ne_simple_propfind(mSession, uri.c_str(), depth, fetchProps, getProps, props);
  if(res!=NE_OK){ 
    mError = ne_get_error(mSession);      
    if(mError.find("404")!= std::string::npos){
      return false;
    }
    return false;
     
  } 
  props->erase(props->begin());
  return true;

}

/**
 * @brief Pulls the whole subtree of webdav-uri
 *
 **/
std::vector<WebdavPath> WebDavClient::tree(std::string uri){
  std::vector<WebdavPath> *props = new std::vector<WebdavPath>;
  const int depth = NE_DEPTH_INFINITE; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
  int res = ne_simple_propfind(mSession, uri.c_str(), depth, fetchProps, WebDavClient::getProps, props);
  if(res!=NE_OK){ 
    mError = ne_get_error(mSession);
    return *props;
   } 

  return *props;
}

/**
 * @brief Puts a file (localSource) on a directory(uri)of the
 *       webdav-server.
 *
 * @param uri         destination on the webdav-server
 *        localSource sourcefile, that will be copied
 *
 * @return false if can´t put file
 *         true  otherwise
 **/
bool WebDavClient::put(std::string uri, std::string localSource){
  int fd = open(localSource.c_str(), O_RDONLY); 
  int res = ne_put(mSession, uri.c_str(), fd); 
  if(res!=NE_OK){ 
    mError = ne_get_error(mSession);
    return false;
  } 
  close(fd);
  return true;
}

/**
 * @brief Downloads a file from webdav-server(uri).
 *
 * @param uri              source of file to download
 *        localDestination download-destination
 *
 * @return false if can´t download file
 *         true  otherwise
 **/
bool WebDavClient::get(std::string uri, std::string localDestination){
  int fd = open(localDestination.c_str(), O_WRONLY | O_CREAT); 
  int res = ne_get(mSession, uri.c_str(), fd); 
  if(res!=NE_OK){ 
    mError = ne_get_error(mSession);
    return false;
  } 
  close(fd);
  return true;

}

/**
 * @brief Creates a directory on webdav-server at uri
 *
 * @param uri directory to create
 *
 * @return false if can´t create directory
 *         true  otherwise
 **/
bool WebDavClient::mkdir(std::string uri){
  int res = ne_mkcol(mSession, uri.c_str());
  if(res != NE_OK){ 
    mError = ne_get_error(mSession);
    return false;
  }
  return true;
}

/**
 * @brief Delete file or directory on webdav-server at uri
 *
 * @param uri directory to delete
 *
 * @return false if can´t delete directory
 *         true  otherwise
 **/
bool WebDavClient::rm(std::string uri){
  int res = ne_delete(mSession, uri.c_str());
  if(res!=NE_OK){
    mError = ne_get_error(mSession);
    return false;
  }
  return true;
}

std::string WebDavClient::getLastError(){
  return mError;
}
#endif
}}}
