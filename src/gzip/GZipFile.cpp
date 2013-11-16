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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "gzip/GZipFile.hpp"

namespace ub {

//------------------------------------------------------------------------------
GZipFile::GZipFile(const QString& name)
	: QIODevice(), fileName(name)
{ }

//------------------------------------------------------------------------------
GZipFile::~GZipFile()
{
	if (isOpen())
	{
		close();
	}
}

//------------------------------------------------------------------------------
bool GZipFile::isSequential() const
{
	return true;
}

//------------------------------------------------------------------------------
bool GZipFile::open(QIODevice::OpenMode mode)
{
	char modeStr[2];
	modeStr[1] = '\0';

	if ((mode & QIODevice::ReadOnly) != 0)
	{
		modeStr[0] = 'r';
	}
	if ((mode & QIODevice::WriteOnly) != 0)
	{
		modeStr[0] = 'w';
	}

	file = gzopen(fileName.toUtf8().data(), modeStr);
	if ( ! file)
		return false;

	return QIODevice::open(mode);
}

//------------------------------------------------------------------------------
void GZipFile::close()
{
	QIODevice::close();
	gzclose(file);
}

//------------------------------------------------------------------------------
qint64 GZipFile::readData(char* data, qint64 maxSize)
{
	int bytesRead = gzread(file, (void*) data, maxSize);
	if (bytesRead < maxSize)
	{
		if (gzeof(file))
			return bytesRead;
		else
		{
			int err;
			const char* errStr = gzerror(file, &err);
			if (err)
			{
				qWarning() << errStr;
				return -1;
			}
		}
	}

	return bytesRead;
}

//------------------------------------------------------------------------------
qint64 GZipFile::writeData(const char* data, qint64 maxSize)
{
	int written = gzwrite(file, (void*) data, maxSize);
	return (written == 0) ? -1 : written;
}

}
