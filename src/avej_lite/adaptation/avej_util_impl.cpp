
#include "../avej_config.h"
#include "../avej_util.h"

////////////////////////////////////////////////////////////////////////////////
// avej_lite::util::GetTicks

#if (TARGET_DEVICE == TARGET_WIN32)

#include <windows.h>
#pragma comment(lib, "winmm.lib")

namespace avej_lite
{
	namespace util
	{
		unsigned long GetTicks(void)
		{
			return timeGetTime();
		}
		void Delay(unsigned long msec)
		{
			Sleep(msec);
		}

	}
}

#elif (TARGET_DEVICE == TARGET_GP2XWIZ)

// delete ?

#elif (TARGET_DEVICE == TARGET_BADA)

#include <FSystem.h>

namespace avej_lite
{
	namespace util
	{
		unsigned long GetTicks(void)
		{
			static long long s_startTick = 0;

			if (s_startTick == 0)
				Osp::System::SystemTime::GetTicks(s_startTick);

			static long long currentTick;
			Osp::System::SystemTime::GetTicks(currentTick);

			double diff = currentTick - s_startTick;

			return (unsigned long)diff;
		}
	}
}

#elif (TARGET_DEVICE == TARGET_TIZEN)

namespace
{
	void SystemTime_GetTicks(long long& tick)
	{
		static long long s_tick = 0;
		tick = s_tick;
		s_tick += 20;
	}
}

namespace avej_lite
{
	namespace util
	{
		unsigned long GetTicks(void)
		{
			static long long s_startTick = 0;

			if (s_startTick == 0)
				SystemTime_GetTicks(s_startTick);

			static long long currentTick;
			SystemTime_GetTicks(currentTick);

			double diff = currentTick - s_startTick;

			return (unsigned long)diff;
		}
	}
}

#else

#error ...

#endif // #if defined(_WIN32)

////////////////////////////////////////////////////////////////////////////////
// avej_lite::util::Random

namespace avej_lite
{
	namespace util
	{
		static unsigned long rand_seed = 0L;

		int Random(int range)
		{
			if (range <= 0)
				return 0;

			rand_seed = rand_seed * 214013L + 2531011L;
			return ((rand_seed >> 16) & 0x7fff) % range;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// avej_lite::util::CReadStream

namespace avej_lite
{
	namespace util
	{
		////////////////////////////////////////////////////////////////////////
		// CReadStream

		CReadStream::CReadStream(void)
			: m_is_available(false)
		{
		}

		CReadStream::~CReadStream(void)
		{
		}

		bool CReadStream::IsValid(void) const
		{
			return m_is_available;
		}
	}
}

#if ( ( TARGET_DEVICE == TARGET_WIN32 ) || ( TARGET_DEVICE == TARGET_GP2XWIZ ) )

#include <stdio.h>

namespace avej_lite
{
	namespace util
	{
		////////////////////////////////////////////////////////////////////////
		// CFileReadStream

		struct CFileReadStream::TImpl
		{
			FILE* m_p_file;

			TImpl()
				: m_p_file(0) {}
			~TImpl()
			{
				if (m_p_file)
					fclose(m_p_file);
			}
		};

		CFileReadStream::CFileReadStream(const char* sz_file_name)
			: m_p_impl(new TImpl)
		{
			string file_name = s_app_path;

			if (!file_name.isEmpty())
				file_name += "/";

			file_name += sz_file_name;
			
			m_p_impl->m_p_file = fopen(file_name, "rb");

			this->m_is_available = (m_p_impl->m_p_file != 0);
		}

		CFileReadStream::~CFileReadStream(void)
		{
			delete m_p_impl;
		}

		long  CFileReadStream::Read(void* p_buffer, long count) const
		{
			if (!this->m_is_available)
				return 0;

			return fread(p_buffer, 1, count, m_p_impl->m_p_file);
		}

		long  CFileReadStream::Seek(long offset, TOrigin origin) const
		{
			if (!this->m_is_available)
				return -1;

			switch (origin)
			{
			case SEEK_SET:
			case SEEK_CUR:
			case SEEK_END:
				fseek(m_p_impl->m_p_file, offset, origin);
				break;
			default:
				return -1;
			}

			return ftell(m_p_impl->m_p_file);
		}

		long  CFileReadStream::GetSize(void) const
		{
			if (!this->m_is_available)
				return -1;

			long  Result;
			long  CurrentPos = ftell(m_p_impl->m_p_file);

			fseek(m_p_impl->m_p_file, 0, SEEK_END);
			Result = ftell(m_p_impl->m_p_file);

			fseek(m_p_impl->m_p_file, CurrentPos, SEEK_SET);

			return Result;
		}

		void* CFileReadStream::GetPointer(void) const
		{
			return 0;
		}

		bool  CFileReadStream::IsValidPos(void) const
		{
			if (!this->m_is_available)
				return false;

			return (feof(m_p_impl->m_p_file) == 0);
		}
	}
}

#elif ( TARGET_DEVICE == TARGET_BADA )

#include <FIo.h>

namespace avej_lite
{
	namespace util
	{
		////////////////////////////////////////////////////////////////////////
		// CFileReadStream

		struct CFileReadStream::TImpl
		{
			Osp::Io::File file;
			int           file_length;
		};

		CFileReadStream::CFileReadStream(const char* sz_file_name)
			: m_p_impl(new TImpl)
		{
			Osp::Base::String file_name(sz_file_name);

			if (!s_app_path.isEmpty())
			{
				Osp::Base::String app_path(s_app_path);
				file_name = "/Res/" + app_path + "/" + file_name;
			}
			else
			{
				file_name = "/Res/" + file_name;
			}


			result r = m_p_impl->file.Construct(file_name, L"r");

			if (r != E_SUCCESS)
				return;

			m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_END, 0);
			m_p_impl->file_length = m_p_impl->file.Tell();
			m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_BEGIN, 0);

			this->m_is_available = true;
		}

		CFileReadStream::~CFileReadStream(void)
		{
			delete m_p_impl;
		}

		long  CFileReadStream::Read(void* p_buffer, long count) const
		{
			if (!this->m_is_available)
				return 0;

			m_p_impl->file.Read(p_buffer, count);
		}

		long  CFileReadStream::Seek(long offset, TOrigin origin) const
		{
			if (!this->m_is_available)
				return -1;

			switch (origin)
			{
			case ORIGN_SET:
				m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_BEGIN, offset);
				break;
			case ORIGN_CUR:
				m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_CURRENT, offset);
				break;
			case ORIGN_END:
				m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_END, offset);
				break;
			default:
				return -1;
			}

			return m_p_impl->file.Tell();
		}

		long  CFileReadStream::GetSize(void) const
		{
			if (!this->m_is_available)
				return -1;

			return m_p_impl->file_length;
		}

		void* CFileReadStream::GetPointer(void) const
		{
			return 0;
		}

		bool  CFileReadStream::IsValidPos(void) const
		{
			if (!this->m_is_available)
				return false;

			int read_pos = m_p_impl->file.Tell();

			return (read_pos >= 0) && (read_pos < m_p_impl->file_length);
		}
	}
}

#elif ( TARGET_DEVICE == TARGET_TIZEN )

#include <stdio.h>
#include <string.h>
#include <dlog.h>

#define g_printLog(s) dlog_print(DLOG_INFO, "[SMGAL]", s);

namespace
{
	template <typename T>
	T getMin(T a, T b)
	{
		return (a < b) ? a : b;
	}
}

namespace avej_lite
{
	namespace util
	{
		string s_app_path1;
		string s_app_path2;

		void SetAppPath(const string& path)
		{
			s_app_path1 = path;
		}

		void SetResourcePath(const string& path)
		{
			s_app_path2 = path;
		}

		////////////////////////////////////////////////////////////////////////
		// CFileReadStream

		struct CFileReadStream::TImpl
		{
			unsigned char* m_p_buffer;
			unsigned int m_buffer_length;
			unsigned int m_offset;
			bool  m_is_available;

			TImpl()
				: m_p_buffer(0)
				, m_buffer_length(0)
				, m_offset(0)
				, m_is_available(false)
			{
			}

			~TImpl()
			{
				delete[] m_p_buffer;
			}
		};

		CFileReadStream::CFileReadStream(const char* sz_file_name)
			: m_p_impl(new TImpl)
		{
			string file_name;

			file_name  = s_app_path1;
			file_name += s_app_path2;
			file_name += sz_file_name;

			{
				FILE* p_file = fopen(file_name, "rb");
				if (p_file)
				{
					fseek(p_file, 0, SEEK_END);
					m_p_impl->m_buffer_length = ftell(p_file);
					fseek(p_file, 0, SEEK_SET);
					m_p_impl->m_p_buffer = new unsigned char[m_p_impl->m_buffer_length];

					fread(m_p_impl->m_p_buffer, m_p_impl->m_buffer_length, 1, p_file);

					fclose(p_file);
				}
			}

			m_p_impl->m_offset = 0;
			m_p_impl->m_is_available = (m_p_impl->m_p_buffer != 0);

			if (m_p_impl->m_p_buffer)
			{
				char sz_log[256];
				sprintf(sz_log, "[SMGAL] %s found!", (const char*)file_name);
				g_printLog(sz_log);
			}
			else
			{
				char sz_log[256];
				sprintf(sz_log, "[SMGAL] %s NOT found!", (const char*)file_name);
				g_printLog(sz_log);
			}

			this->m_is_available = m_p_impl->m_is_available;
		}

		CFileReadStream::~CFileReadStream(void)
		{
			delete m_p_impl;
		}

		long  CFileReadStream::Read(void* p_buffer, long count) const
		{
			if (!m_p_impl->m_is_available)
				return 0;

			long copied_bytes = getMin(count, long(m_p_impl->m_buffer_length - m_p_impl->m_offset));

			if (copied_bytes <= 0)
				return 0;

			memcpy(p_buffer, &m_p_impl->m_p_buffer[m_p_impl->m_offset], copied_bytes);

			m_p_impl->m_offset += copied_bytes;

			return copied_bytes;
		}

		long  CFileReadStream::Seek(long offset, TOrigin origin) const
		{
			if (!m_p_impl->m_is_available)
				return -1;

			switch (origin)
			{
			case ORIGN_SET:
				m_p_impl->m_offset = offset;
				break;
			case ORIGN_CUR:
				m_p_impl->m_offset += offset;
				break;
			case ORIGN_END:
				m_p_impl->m_offset = m_p_impl->m_buffer_length + offset;
				break;
			default:
				return -1;
			}

			return m_p_impl->m_offset;
		}

		long  CFileReadStream::GetSize(void) const
		{
			if (!m_p_impl->m_is_available)
				return -1;

			return m_p_impl->m_buffer_length;
		}

		void* CFileReadStream::GetPointer(void) const
		{
			return 0;
		}

		bool  CFileReadStream::IsValidPos(void) const
		{
			if (!m_p_impl->m_is_available)
				return false;

			return (m_p_impl->m_offset < m_p_impl->m_buffer_length);
		}
	}
}

#else

#error ...

#endif

////////////////////////////////////////////////////////////////////////////////
// CMemoryReadStream

namespace avej_lite
{
	namespace util
	{
		struct CMemoryReadStream::TImpl
		{
			void* p_memory;
			long  size;
			long  position;
		};

		CMemoryReadStream::CMemoryReadStream(void* p_memory, long size)
			: m_p_impl(new TImpl)
		{
			m_p_impl->p_memory = p_memory;
			m_p_impl->size     = size;
			m_p_impl->position = 0;

			m_is_available = (m_p_impl->p_memory != 0);
		}

		CMemoryReadStream::~CMemoryReadStream()
		{
			delete m_p_impl;
		}

		long CMemoryReadStream::Read(void* p_buffer, long count) const
		{
			if (!this->m_is_available)
				return -1;

			long result;

			if ((m_p_impl->position >= 0) && (count >= 0 ))
			{
				result = m_p_impl->size - m_p_impl->position;
				if (result > 0)
				{
					if (result > count)
						result = count;

					memcpy(p_buffer, (char*)m_p_impl->p_memory + m_p_impl->position, result);

					m_p_impl->position += result;

					return result;
				}
			}

			return 0;
		}

		long CMemoryReadStream::Seek(long offset, TOrigin origin) const
		{
			if (!this->m_is_available)
				return -1;

			switch (origin)
			{
			case ORIGN_SET:
				m_p_impl->position = offset;
				break;
			case ORIGN_CUR:
				m_p_impl->position += offset;
				break;
			case ORIGN_END:
				m_p_impl->position = m_p_impl->size + offset;
				break;
			default:
				return -1;
			}

			return m_p_impl->position;
		}

		long CMemoryReadStream::GetSize(void) const
		{
			if (!this->m_is_available)
				return -1;

			return m_p_impl->size;
		}

		void *CMemoryReadStream::GetPointer(void) const
		{
			if (!this->m_is_available)
				return 0;

			return m_p_impl->p_memory;
		}

		bool CMemoryReadStream::IsValidPos(void) const
		{
			if (!this->m_is_available)
				return false;

			return ((m_p_impl->position >= 0) && (m_p_impl->position < m_p_impl->size));
		}
	}
}
