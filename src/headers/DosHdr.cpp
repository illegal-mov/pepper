#include "DosHdr.h"

using namespace Pepper;

const char* DosHeader::getFieldName(const int index)
{
    switch (index) {
        case E_MAGIC   : return "Signature";
        case E_CBLP    : return "Count of Bytes on Last Page";
        case E_CP      : return "Count of Pages";
        case E_CRLC    : return "Count of Relocations";
        case E_CPARHDR : return "Count of Paragraph Headers";
        case E_MINALLOC: return "Minimum Allocations";
        case E_MAXALLOC: return "Maximum Allocations";
        case E_SS      : return "Initial SS";
        case E_SP      : return "Initial SP";
        case E_CSUM    : return "Checksum";
        case E_IP      : return "Initial IP";
        case E_CS      : return "Initial CS";
        case E_LFARLC  : return "Offset to Relocations";
        case E_OVNO    : return "Overlay Number";
        case E_RES     : return "Reserved1";
        case E_OEMID   : return "OEM Identifier";
        case E_OEMINFO : return "OEM Info";
        case E_RES2    : return "Reserved2";
        case E_LFANEW  : return "File Offset to NT Header";
        default        : return "<UNKNOWN>";
    }
}

const void* DosHeader::getFieldPtr(const int index) const
{
    switch (index) {
        case E_MAGIC   : return &getStructPtr()->e_magic;
        case E_CBLP    : return &getStructPtr()->e_cblp;
        case E_CP      : return &getStructPtr()->e_cp;
        case E_CRLC    : return &getStructPtr()->e_crlc;
        case E_CPARHDR : return &getStructPtr()->e_cparhdr;
        case E_MINALLOC: return &getStructPtr()->e_minalloc;
        case E_MAXALLOC: return &getStructPtr()->e_maxalloc;
        case E_SS      : return &getStructPtr()->e_ss;
        case E_SP      : return &getStructPtr()->e_sp;
        case E_CSUM    : return &getStructPtr()->e_csum;
        case E_IP      : return &getStructPtr()->e_ip;
        case E_CS      : return &getStructPtr()->e_cs;
        case E_LFARLC  : return &getStructPtr()->e_lfarlc;
        case E_OVNO    : return &getStructPtr()->e_ovno;
        case E_RES     : return &getStructPtr()->e_res;
        case E_OEMID   : return &getStructPtr()->e_oemid;
        case E_OEMINFO : return &getStructPtr()->e_oeminfo;
        case E_RES2    : return &getStructPtr()->e_res2;
        case E_LFANEW  : return &getStructPtr()->e_lfanew;
        default        : return nullptr;
    }
}
