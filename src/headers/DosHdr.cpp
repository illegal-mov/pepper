#include "DosHdr.h"

using namespace Pepper;

const char* DosHeader::getFieldName(int index)
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

const void* DosHeader::getFieldPtr(int index) const
{
    switch (index) {
        case E_MAGIC   : return &dos()->e_magic;
        case E_CBLP    : return &dos()->e_cblp;
        case E_CP      : return &dos()->e_cp;
        case E_CRLC    : return &dos()->e_crlc;
        case E_CPARHDR : return &dos()->e_cparhdr;
        case E_MINALLOC: return &dos()->e_minalloc;
        case E_MAXALLOC: return &dos()->e_maxalloc;
        case E_SS      : return &dos()->e_ss;
        case E_SP      : return &dos()->e_sp;
        case E_CSUM    : return &dos()->e_csum;
        case E_IP      : return &dos()->e_ip;
        case E_CS      : return &dos()->e_cs;
        case E_LFARLC  : return &dos()->e_lfarlc;
        case E_OVNO    : return &dos()->e_ovno;
        case E_RES     : return &dos()->e_res;
        case E_OEMID   : return &dos()->e_oemid;
        case E_OEMINFO : return &dos()->e_oeminfo;
        case E_RES2    : return &dos()->e_res2;
        case E_LFANEW  : return &dos()->e_lfanew;
        default        : return nullptr;
    }
}
