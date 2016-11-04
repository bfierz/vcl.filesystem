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
#include <fstream>

// Include the relevant parts from the library
#include <vcl/filesystem/mountpoints/archivemountpoint.h>
#include <vcl/filesystem/mountpoints/volumemountpoint.h>
#include <vcl/filesystem/filesystem.h>
#include <vcl/filesystem/util/archive.h>

// Google test
#include <gtest/gtest.h>

class SimpleFileSystemTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		namespace fs = std::experimental::filesystem;

		ASSERT_FALSE(exists(sample_dir)) << "Sample content directory must not exists.";
		ASSERT_TRUE(fs::create_directory(sample_dir)) << "Sample content directory could not be created.";
		ASSERT_TRUE(fs::create_directory(sample_sub_dir)) << "Sample content could not be created.";

		const fs::path sample_file_1{ sample_dir / "SubText.txt" };
		const fs::path sample_file_2{ sample_dir / "File2.txt" };
		const fs::path sample_file_3{ sample_sub_dir / "File3.txt" };

		std::ofstream file1{ sample_file_1.string() };
		ASSERT_TRUE(file1.is_open()) << "Sample content could not be created.";
		file1 << "File1";
		file1.close();

		std::ofstream file2{ sample_file_2.string() };
		ASSERT_TRUE(file2.is_open()) << "Sample content could not be created.";
		file2 << "File2";
		file2.close();

		std::ofstream file3{ sample_file_3.string() };
		ASSERT_TRUE(file3.is_open()) << "Sample content could not be created.";
		file3 << "File3";
		file3.close();
	}

	void TearDown() override
	{
		namespace fs = std::experimental::filesystem;

		const fs::path sample_dir{ "SampleContent" };
		ASSERT_TRUE(exists(sample_dir)) << "Sample content directory must exists.";
		ASSERT_EQ(fs::remove_all(sample_dir), 3);
	}

	const std::experimental::filesystem::path sample_dir{ "SampleContent" };
	const std::experimental::filesystem::path sample_sub_dir{ "SampleContent/SubText" };
};

TEST_F(SimpleFileSystemTest, VolumePathExistence)
{
	using namespace Vcl::FileSystem;

	FileSystem fs;
	fs.addMountPoint(std::make_unique<VolumeMountPoint>("Basics", "/texts", "SampleContent"));
	fs.addMountPoint(std::make_unique<VolumeMountPoint>("Basics", "/texts/subs", "SampleContent/SubText"));

	EXPECT_TRUE(fs.exists("/texts/File2.txt"));
	EXPECT_TRUE(fs.exists("/texts/SubText.txt"));
	EXPECT_TRUE(fs.exists("/texts/subs/File3.txt"));
}

TEST_F(SimpleFileSystemTest, ReadVolumeFile)
{
	using namespace Vcl::FileSystem;

	FileSystem fs;
	fs.addMountPoint(std::make_unique<VolumeMountPoint>("Basics", "/texts", "SampleContent"));
	fs.addMountPoint(std::make_unique<VolumeMountPoint>("Basics", "/texts/subs", "SampleContent/SubText"));

	auto reader = fs.createReader("/texts/File2.txt");

	char text[128];
	auto read_bytes = reader->read(text, sizeof(text));
	text[5] = 0;

	EXPECT_STREQ(text, "File2");
}

TEST(FileSystemTest, ArchivePathExistence)
{
	using namespace Vcl::FileSystem;

	FileSystem fs;
	fs.addMountPoint(std::make_unique<ArchiveMountPoint>("Content", "/content", "simple.zip"));
	fs.addMountPoint(std::make_unique<ArchiveMountPoint>("SubContent", "/content/sub", "simple.zip"));

	EXPECT_TRUE(fs.exists("/content/simple.txt"));
	EXPECT_TRUE(fs.exists("/content/sub/simple.txt"));
	EXPECT_TRUE(fs.exists("/content/sub/test/test.txt"));
}

TEST(FileSystemTest, ListZipFileContent)
{
	using namespace Vcl::FileSystem::Util;

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

TEST(FileSystemTest, ReadArchiveFile)
{
	using namespace Vcl::FileSystem;

	FileSystem fs;
	fs.addMountPoint(std::make_unique<ArchiveMountPoint>("Content", "/content", "simple.zip"));

	auto reader = fs.createReader("/content/simple.txt");

	char text[128];
	auto read_bytes = reader->read(text, sizeof(text));
	text[6] = 0;

	EXPECT_STREQ(text, "Simple");
}
