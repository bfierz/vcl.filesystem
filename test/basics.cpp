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

// VCL configuration
#include <vcl/config/global.h>

// Include the relevant parts from the library
#include <vcl/filesystem/mountpoints/volumemountpoint.h>
#include <vcl/filesystem/filesystem.h>
#include <vcl/filesystem/util/archive.h>

// Google test
#include <gtest/gtest.h>

TEST(FileSystemTest, PathExistence)
{
	using namespace Vcl::FileSystem;

	FileSystem fs;
	fs.addMountPoint("", std::make_unique<VolumeMountPoint>("Basics", "C:\\texts", "/texts"));
	fs.addMountPoint("", std::make_unique<VolumeMountPoint>("Basics", "C:\\subtexts", "/texts/subs"));

	fs.exists("/texts/file.txt");
	fs.exists("/texts/subs.txt");
	fs.exists("/texts/subs/file2.txt");
}

TEST(FileSystemTest, ListZipFileContent)
{
	using namespace Vcl::FileSystem::Zip;

	// Archive to check
	Archive ar{ "simple.zip" };

	// Reference content of the file
	std::set<std::experimental::filesystem::path> files =
	{
		"simple.txt",
		"test/",
		"test/test.txt"
	};

	// Check if all the files in the archive are in the reference list
	for (auto it = ar.beginPaths(); it != ar.endPaths(); ++it)
	{
		EXPECT_TRUE(files.find(*it) != files.end()) << "Missing " << *it;
	}

	// Check if all the files in the reference list are in the archive
	for (const auto& p : files)
	{
		EXPECT_TRUE(ar.entryExists(p)) << "Missing " << p;
	}
}