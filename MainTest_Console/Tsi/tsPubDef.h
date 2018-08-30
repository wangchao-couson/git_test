#ifndef _ts_pubdef_h_
#define _ts_pubdef_h_


// ////////////////////////////////////////////////////////////////////////
#ifdef _WIN32

    #include <Windows.h>
    #include <crtdbg.h>
    #include <stdio.h>

    #include "safestring.h"

    #pragma warning(disable:4786)
    #pragma warning(disable:4200)

#else // !_WIN32

#endif// _WIN32


// ////////////////////////////////////////////////////////////////////////
#define ns_begin_tscCommon  namespace tscCommon {
#define ns_tscCommon        tscCommon

#define ns_begin_tscCrypt   namespace tscCrypt {
#define ns_tscCrypt         tscCrypt

#define ns_begin_tscHash    namespace tscHash {
#define ns_tscHash          tscHash

#define ns_begin_tscMath    namespace tscMath {
#define ns_tscMath          tscMath

#define ns_begin_tscPack    namespace tscPack {
#define ns_tscPack          tscPack

#define ns_begin_tscNet     namespace tscNet {
#define ns_tscNet           tscNet

#define ns_begin_tscTsu     namespace tscTsu {
#define ns_tscTsu           tscTsu

#define ns_begin_tscTmu     namespace tscTmu {
#define ns_tscTmu           tscTmu

#define ns_begin_tscTuc     namespace tscTuc {
#define ns_tscTuc           tscTuc

#define ns_begin_tscDllLoader   namespace tscDllLoader {
#define ns_tscDllLoader     tscDllLoader

#define ns_begin_tscIo      namespace tscIo {
#define ns_tscIo            tscIo

#define ns_begin_tscBApi    namespace tscBApi {
#define ns_tscBApi          tscBApi

#define ns_begin_tscIpc     namespace tscIpc {
#define ns_tscIpc           tscIpc

#define ns_begin_tscMap     namespace tscMap {
#define ns_tscMap           tscMap

#define ns_begin_tscXml     namespace tscXml {
#define ns_tscXml           tscXml

#define ns_begin_tscHook    namespace tscHook {
#define ns_tscHook          tscHook

#define ns_begin_tscDisam   namespace tscDisam {
#define ns_tscDisam         tscDisam

#define ns_end              };

// ////////////////////////////////////////////////////////////////////////
#define TS_INLINE   __forceinline

#define TS_IN
#define TS_OUT
#define TS_IN_OUT

#define TS_ASSERT(x)    _ASSERT(x)
#define TS_ASSERTE(x)   _ASSERTE(x)

#define TS_DO           do {
#define TS_WHILE        } while (0)
#define TS_BREAK        break
#define TS_CONTINUE     continue

#ifndef _countof
    #define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#define TS_SAFE_NEW(a, b, size)           if (!a) {a = (b*)new unsigned char[size]; memset((void*)a, 0, size);}

#define TS_SAFE_DELETE(a)               if (a) {delete (a); (a) = 0;}
#define TS_SAFE_DELETE_ARR(a)           if (a) {delete[] (a); (a) = 0;}
#define TS_SAFE_STRUCT_ARR_DELETE(a, b)  if (a) {TS_SAFE_DELETE_ARR(b);}

#ifdef _WIN32
    #define TS_SAFE_CLOSE_HANDLE(a)     if ((a) != NULL) {::CloseHandle(a); (a) = NULL;}
    #define TS_SAFE_CLOSE_FILE(a)       if ((a) != INVALID_HANDLE_VALUE) {::CloseHandle(a); (a) = INVALID_HANDLE_VALUE;}
#endif // _WIN32

#define TS_SAFE_CHECK_VALUE         TS_SAFE_CHECK_PTR
#define TS_SAFE_CHECK_PTR(a)        TS_ASSERT(a); if (!a) {return ;}
#define TS_SAFE_CHECK_VALUE_RET     TS_SAFE_CHECK_PTR_RET
#define TS_SAFE_CHECK_PTR_RET(a, b) TS_ASSERT(a); if (!a) {return b;}


#define TS_CLASS_UNCOPYABLE(ClassName) \
    private: \
    ClassName##(const ClassName##&); \
    ClassName##& operator=(const ClassName##&);


#define TS_DECLARE_PROPERTY(type, x) \
    protected:  \
    type m_##x; \
    public: \
    type Get##x() const {return m_##x;} \
    void Set##x(type x) {m_##x = x;}


// ////////////////////////////////////////////////////////////////////////
// 自定义的buf结构，带不同字节的长度

typedef struct tagTS_BUFB
{
    BYTE bySize;
    BYTE byBuf[];
} TS_BUFB;
#define TSBUFB_MAX_SIZE(a)      ((a)->bySize + sizeof(BYTE))

typedef struct tagTS_BUFW
{
    WORD wSize;
    BYTE byBuf[];
} TS_BUFW;
#define TSBUFW_MAX_SIZE(a)      ((a)->wSize + sizeof(WORD))

typedef struct tag1
{
    DWORD dwSize;
    BYTE byBuf[];
} TS_BUFD;
#define TSBUFD_MAX_SIZE(a)      ((a)->dwSize + sizeof(DWORD))


#endif// _ts_pubdef_h_

