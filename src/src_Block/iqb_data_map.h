
#ifndef __IQB_DATA_MAP_H__
#define __IQB_DATA_MAP_H__

#include "iqb_base_type.h"

namespace erio
{
	struct TMapData
	{
		int tile;
		int wall;
	};

	class CTileMap
	{
	private:
		int m_x_map_size;
		int m_y_map_size;
		int m_x_map_pitch;
		int m_x_offset;
		int m_y_offset;

		TMapData* m_p_map_data;

	public:
		CTileMap(const char* map_data[], int x_offset = 0, int y_offset = 0)
			: m_x_map_size(0)
			, m_y_map_size(0)
			, m_x_map_pitch(0)
			, m_x_offset(x_offset)
			, m_y_offset(y_offset)
			, m_p_map_data(0)
		{
			Init(map_data);
		}

		~CTileMap(void)
		{
			delete[] m_p_map_data;
		}

		void Init(const char* map_data[], int x_offset = 0, int y_offset = 0)
		{
			m_x_map_size = strlen(map_data[0]);
			m_y_map_size = 0;

			while (map_data[m_y_map_size])
				++m_y_map_size;

			m_x_map_pitch = m_x_map_size;
			m_x_offset = x_offset;
			m_y_offset = y_offset;

			delete m_p_map_data;

			m_p_map_data = new TMapData[m_x_map_pitch * m_y_map_size];

			for (int j = 0; j < m_y_map_size; j++)
			{
				TMapData* p_map = m_p_map_data;
				p_map += j * m_x_map_pitch;

				for (int i = 0; i < m_x_map_size; i++)
				{
					int data = map_data[m_y_map_size-1-j][i] - '0';

					if (data < 1)
					{
						p_map->tile = data;
						p_map->wall = 0;
					}
					else
					{
						p_map->tile = 0;
						p_map->wall = data;
					}

					++p_map;
				}
			}
		}

		TMapData* m_GetMapData(int& out_x, int& out_y) const
		{
			out_x -= m_x_offset;
			out_y -= m_y_offset;

			if ((out_x < 0) || (out_x >= m_x_map_size) || (out_y < 0) || (out_y >= m_y_map_size))
				return NULL;

			if (m_p_map_data == NULL)
				return NULL;

			TMapData* p_map = m_p_map_data;
			p_map += out_y * m_x_map_pitch + out_x;

			return p_map;
		}


		bool IsWallMap(int x, int y) const
		{
			TMapData* p_map = m_GetMapData(x, y);

			if (p_map)
				return (p_map->wall > 0);
			else
				return false;
		}

		bool IsMovableMap(int x, int y) const
		{
			TMapData* p_map = m_GetMapData(x, y);

			if (p_map)
				return (p_map->wall == 0);
			else
				return true;
		}

		bool IsMovableMapRect(float fx, float fy, float radius) const
		{
			int xTest, yTest;

			xTest = int(fx       +10000.f)-10000;
			yTest = int(fy       +10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx-radius+10000.f)-10000;
			yTest = int(fy       +10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx+radius+10000.f)-10000;
			yTest = int(fy       +10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx       +10000.f)-10000;
			yTest = int(fy-radius+10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx       +10000.f)-10000;
			yTest = int(fy+radius+10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx-radius+10000.f)-10000;
			yTest = int(fy-radius+10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx+radius+10000.f)-10000;
			yTest = int(fy-radius+10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx-radius+10000.f)-10000;
			yTest = int(fy+radius+10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			xTest = int(fx+radius+10000.f)-10000;
			yTest = int(fy+radius+10000.f)-10000;
			if (!IsMovableMap(xTest, yTest))
				return false;

			return true;
		}

		int  GetTile(int x, int y) const
		{
			TMapData* p_map = m_GetMapData(x, y);

			if (p_map)
				return p_map->tile;
			else
				return 0;
		}

		int  GetWall(int x, int y) const
		{
			TMapData* p_map = m_GetMapData(x, y);

			if (p_map)
				return p_map->wall;
			else
				return 0;
		}

		int  GetMapSizeX(void) const
		{
			return m_x_map_size;
		}

		int  GetMapSizeY(void) const
		{
			return m_y_map_size;
		}

	};

} // namespace erio

#endif // #ifndef __IQB_DATA_MAP_H__
