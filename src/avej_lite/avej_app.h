
#ifndef __AVEJ_APP_H
#define __AVEJ_APP_H

/*!
 * \file
 * \ingroup AVEJ library
 * \brief graphics interface of AVEJ library
 * \author SMgal smgal@hitel.net
 * \date 2006-12-31
*/

#define THIS_CLASS_IS_A_SINGLETON(class_name)      \
	friend class ::avej_lite::singleton<class_name>;    \
	friend class ::avej_lite::auto_deletor<class_name>; \
	class_name();                                  \
	class_name(const class_name&);                 \
	~class_name();                                 \
	class_name& operator=(const class_name&)

#define THIS_STRUCT_IS_A_SINGLETON(struct_name)    \
	private:                                       \
	THIS_CLASS_IS_A_SINGLETON(struct_name);        \
	public:                                        \


namespace avej_lite
{
	template <typename TBaseClass>
	class auto_deletor
	{
	public:
		auto_deletor(TBaseClass* object = 0)
			: m_object(object)
		{
		}
		~auto_deletor(void)
		{
			delete m_object;
		}
		void set(TBaseClass* object)
		{
			m_object = object;
		}

	private:
		TBaseClass* m_object;
	};

	//! template library for creating singletone instance (single thread only)
	/*!
	 * \ingroup gfx2d
	 */
	template <typename TBaseClass, int kind = 0>
	class singleton
	{
	public:
		static TBaseClass& get(int param = 0)
		{
			if (m_instance == 0)
			{
				m_instance = new TBaseClass;
				m_deletor.set(m_instance);
			}

			return *m_instance;
		}
		// when explicit releasing order is needed.
		static void release(int param = 0)
		{
			if (m_instance)
			{
				delete m_instance;
				m_instance = 0;
				m_deletor.set(m_instance);
			}
		}
	private:
		singleton(void);

		static TBaseClass* m_instance;
		static auto_deletor<TBaseClass> m_deletor;
	};

	template <class TBaseClass, int kind> TBaseClass* singleton<TBaseClass, kind>::m_instance = 0;
	template <class TBaseClass, int kind> auto_deletor<TBaseClass> singleton<TBaseClass, kind>::m_deletor;
} // namespace avej_lite

namespace avej_lite
{
	////////////////////////////////////////////////////////////////////////////
	// class CInputDevice

	//! key definition of the input device
	enum TInputKey
	{
		INPUT_KEY_BEGIN = 0,  //!< fisrt value of key definition
		INPUT_KEY_UP    = 0,  //!< up
		INPUT_KEY_DOWN,       //!< down
		INPUT_KEY_LEFT,       //!< left
		INPUT_KEY_RIGHT,      //!< right
		INPUT_KEY_A,          //!< button A
		INPUT_KEY_B,          //!< button B
		INPUT_KEY_C,          //!< button C
		INPUT_KEY_D,          //!< button D
		INPUT_KEY_L1,         //!< left trigger 1
		INPUT_KEY_R1,         //!< right trigger 1
		INPUT_KEY_SYS1,       //!< system button 1 (ex> start)
		INPUT_KEY_SYS2,       //!< system button 2 (ex> select)
		INPUT_KEY_SYS3,       //!< system button 3 (ex> vol up)
		INPUT_KEY_SYS4,       //!< system button 4 (ex> vol down)
		INPUT_KEY_END,        //!< last value of key definition (it is invalid)
		INPUT_KEY_MAX = INPUT_KEY_END - INPUT_KEY_BEGIN
	};

	struct TMultiTouchInfo
	{
		int x, y;

		TMultiTouchInfo()
			: x(-1), y(-1)
		{
		}
	};

	enum { MAX_MULTI_TOUCH = 10 };

	typedef TMultiTouchInfo TMultiTouchInfoArray[MAX_MULTI_TOUCH];
	/*!
	 * \ingroup gfx2d
	 */
	class CInputDevice
	{
		friend class singleton<CInputDevice>;
		friend class auto_deletor<CInputDevice>;

	public:
		//! update state for the currrent input device
		void UpdateInputState();
		//! whether the specified key is held down
		bool IsKeyHeldDown(TInputKey key) const;
		//! whether the specified key is pressed before
		bool WasKeyPressed(TInputKey key) const;
		//!
		bool GetMultiTouchInfo(TMultiTouchInfoArray& multi_touch_info_array) const;

	private:
		CInputDevice();
		~CInputDevice();
	};

} // namespace avej_lite


namespace avej_lite
{

//! callback for event-driven
/*!
 * \ingroup AVEJ library application
*/
struct AppCallback
{
	//! application is creating
	bool (*OnCreate)(unsigned long param);
	//! application is destroying
	bool (*OnDestory)(void);
	//! application is in progress
	bool (*OnProcess)(void);
};

//! interface for an applicaton
/*!
 * \ingroup AVEJ library application
*/
class IAvejApp
{
public:
	virtual          ~IAvejApp(void) {};
	virtual bool      Process(void) = 0;
	static  void      ProcessMessages(void);
	static  IAvejApp* GetInstance(const AppCallback& callBack);
	static  IAvejApp* GetInstance(const AppCallback& callBack, unsigned long param1);
    static  int       Release(void);
};

class IAvejAppTarget: IAvejApp
{
public:
	IAvejAppTarget(void);
	~IAvejAppTarget(void);

	void  Create(void);
	void  Destory(void);
	bool  Process(void);
};

}

#endif // #ifndef __AVEJ_APP_H
