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
#pragma once

// VCL configuration
#include <vcl/config/global.h>

// C++ standard library
#include <string>
#include <vector>

// Neon library
#include <ne_props.h>

namespace Vcl { namespace FileSystem { namespace Util
{
	class WebDavPath
	{
	public:
		WebDavPath(std::string host, std::string path, std::string ressourceType, std::string lastModified, std::string contentType) :
		host(host),
		path(path),
		ressourceType(ressourceType),
		lastModified(lastModified),
		contentType(contentType)
		{
		}
  
		bool isFile() const 
		{
			if (!ressourceType.compare(""))
				return true;
			
			return false;
		}

		bool isDirectory()
		{
			return !isFile();
		}

		std::string host;
		std::string path;
		std::string ressourceType;
		std::string lastModified;
		std::string contentType;

		};

	class WebDavClient
	{
	public:
		/*!
		 *	\brief Initial WebDav client
		 *
		 *	Opens a session with ssl authentication.
		 */
		WebDavClient(const std::string url, const unsigned port, const std::string user, const std::string pass);
		~WebDavClient();

	public: // File system commands
		
		/*!
		 *	\brief Lists the directory specified with uri.
		 *         Behauvior is like ls know from unix console.
		 *	\param uri URI of the directory on the server
		 *	\returns a vector of path of the listed directory.
		 */
		std::vector<WebDavPath> listDirectory(const std::string& uri);

	private:
		//! Neon session
		ne_session* _session{ nullptr };

//  std::vector<WebdavPath> ls(std::string uri);
//  std::vector<WebdavPath> tree(std::string uri);
//  std::string getLastError();
//  bool exist(std::string uri);
//  bool put(std::string uri, std::string localSource);
//  bool get(std::string uri, std::string localDestination);
//  bool mkdir(std::string uri);
//  bool rm(std::string uri);
//
//
// private:
//  static void getProps(void *userdata, const ne_uri *uri, const ne_prop_result_set *set);

//  std::string mError;

	};
}}}
