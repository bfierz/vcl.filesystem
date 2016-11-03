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

// C++ Standard Library
#include <filesystem>
#include <unordered_map>
#include <vector>

// VCL File System Library
#include "filereader.h"
#include "mountpoint.h"

namespace Vcl { namespace FileSystem
{
	class FileSystem
	{
		using path = std::experimental::filesystem::path;

	public:
		/*!
		 *	\brief Add a new mount point to the virtual file system
		 */
		void addMountPoint(path prefix, std::unique_ptr<MountPoint> mp);

		 /*!
		  *	\brief Create a new file reader
		  */
		FileReader createReader(path file_name);

		 /*!
		  *	\brief Check if a path exists
		  *	\param entry path to check
		  *	\returns true if the path is found
		  */
		bool exists(const path& entry);

	private:
		/*!
		 *	\brief Find the mount point with the longst overlap with the path
		 *	\param entry path to search the mount point for
		 *	\returns the mount point belonging to the path 'entry'
		 */
		const MountPoint* findMountPoint(path entry);

	private:
		/// Mount points
		std::vector<std::unique_ptr<MountPoint>> _mountPoints;
	};
}}
