/*
 * Copyright 2013 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GZIPFILE_HPP
#define GZIPFILE_HPP

// Qt include(s)
#include <QIODevice>

// zlib include(s)
#include <zlib.h>

namespace ub {

/**
 * A file device implementation for reading
 * and writing gzip'd files.
 *
 * This is a wrapper around the zlib library's
 * functions.
 */
class GZipFile : public QIODevice
{
	Q_OBJECT

public:
	/**
	 * Constructs a new file object to represent the gzip
	 * file with the given name.
	 *
	 * @param[in] name gzip file location
	 */
	GZipFile(const QString& name);

	/**
	 * Destroys the file object, closing it if necessary.
	 */
	virtual ~GZipFile();

	/**
	 * Re-implemented to always return `true`.
	 */
	virtual bool isSequential() const;

	/**
	 * Re-implemented to open the gzip file handle.
	 */
	virtual bool open(QIODevice::OpenMode mode);

	/**
	 * Re-implemented to close the gzip file handle.
	 */
	virtual void close();

protected:
	/**
	 * Re-implemented to read data from the gzip file handle.
	 */
	virtual qint64 readData(char* data, qint64 maxSize);

	/**
	 * Re-implemented to write data to the gzip file handle.
	 */
	virtual qint64 writeData(const char* data, qint64 maxSize);

private:
	/** GZip file location */
	QString fileName;

	/** GZip file handle */
	gzFile file;
};

}

#endif //GZIPFILE_HPP
