//
// Copyright (c) 2010 Neosys
//
// Desigion making
//	1. According to exodus design, file variable stores file name, but cannot store pointer to opened file.
//  2. Every osbread/osbwrite operation reopens a file -> low speed
//  3. We could save opened file pointer into table and keep index of table in file variable.
//	4. The same true for DB connections.
//	5. Interface type to handle all kinds of opened things is 'void *'.
//  6. Handle is added to cache table with destructor functor, 3-4 lines of additional code to write,
//		but significantly simplifies object design.
//
#ifndef MVHANDLES_H
#  define MVHANDLES_H

#include <vector>
#include <string>

namespace exodus {

typedef void * CACHED_HANDLE;
typedef void (* DELETER_AND_DESTROYER )(CACHED_HANDLE);

#define BAD_CACHED_HANDLE	((void *)0)
#define HANDLE_ENTRY_FREE	((void *)0)

class MvHandleEntry
{
  public:
	MvHandleEntry();
	DELETER_AND_DESTROYER deleter;	// =0 means that slot is empty
	CACHED_HANDLE handle;
	std::wstring extra;
};

class MvHandlesCache
{
  public:
	MvHandlesCache();
	int add_handle(CACHED_HANDLE handle_to_opened_file, DELETER_AND_DESTROYER del, std::wstring name);
	//MvHandleEntry & operator [] (int idx)
	//{ 
	//	return conntbl[ idx];
	//}
	CACHED_HANDLE get_handle(int index, std::wstring name);
	void del_handle(int index);
   virtual ~MvHandlesCache();

  private:
	std::vector<MvHandleEntry> conntbl;
};

#ifndef INSIDE_MVHANDLES_CPP
extern
#endif	// INSIDE_MVHANDLES_CPP
MvHandlesCache mv_handles_cache;	// global table (intended usage: mvos.cpp and mvdbpostgres.cpp)

}	// namespace
#endif // MVHANDLES_H
