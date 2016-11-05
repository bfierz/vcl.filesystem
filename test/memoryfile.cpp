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

// C++ standard library
#include <algorithm>

// Include the relevant parts from the library
#include <vcl/filesystem/mountpoints/memorymountpoint.h>
#include <vcl/filesystem/util/memoryfile.h>
#include <vcl/filesystem/filesystem.h>

// Google test
#include <gtest/gtest.h>

TEST(MemoryFileTest, SimpleMemoryFileRW)
{
	using namespace Vcl::FileSystem::Util;

	// Data to test
	std::vector<uint32_t> ref(4000);
	
	int n = { 0 };
	std::generate(ref.begin(), ref.end(), [&n] { return n++; });

	// Write the data to the memory file
	MemoryFile mem_file{ "SampleFile" };
	mem_file.write(0, ref.data(), ref.size() * sizeof(uint32_t));

	// Read the data back
	std::vector<uint32_t> read_back(4000);
	mem_file.read(0, read_back.data(), read_back.size() * sizeof(uint32_t));

	EXPECT_TRUE(std::equal(ref.begin(), ref.end(), read_back.begin(), read_back.end()));
}

TEST(MemoryFileTest, SimpleMemoryFileInFileSystem)
{
	using namespace Vcl::FileSystem;

	FileSystem fs;
	fs.addMountPoint(std::make_unique<MemoryMountPoint>("Basics", "/"));

	// Data to test
	std::vector<uint32_t> ref(4000);

	int n = { 0 };
	std::generate(ref.begin(), ref.end(), [&n] { return n++; });

	// Write the data to the memory file
	auto writer = fs.createWriter("/SampleFile");
	writer->write(ref.data(), ref.size() * sizeof(uint32_t));

	// Read the data back
	std::vector<uint32_t> read_back(4000);
	auto reader = fs.createReader("/SampleFile");
	reader->read(read_back.data(), read_back.size() * sizeof(uint32_t));

	EXPECT_TRUE(std::equal(ref.begin(), ref.end(), read_back.begin(), read_back.end()));
}
