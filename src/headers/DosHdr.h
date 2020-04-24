#ifndef DOS_H
#define DOS_H

#include "../Exceptions.h"
#include "../generics/iHdr.h"

/*
typedef struct _IMAGE_DOS_HEADER {
    char    e_magic[2]; // signature
    int16_t e_cblp;     // count of bytes on last page
    int16_t e_cp;       // count of pages
    int16_t e_crlc;     // count of relocations
    int16_t e_cparhdr;  // count of paragraph headers
    int16_t e_minalloc; // minimum allocations
    int16_t e_maxalloc; // maximum allocations
    int16_t e_ss;       // initial SS
    int16_t e_sp;       // initial SP
    int16_t e_csum;     // checksum
    int16_t e_ip;       // initial IP
    int16_t e_cs;       // initial CS
    int16_t e_lfarlc;   // offset to relocations
    int16_t e_ovno;     // overlay number
    int16_t e_res[4];   // reserved1
    int16_t e_oemid;    // oem Identifier
    int16_t e_oeminfo;  // oem Info
    int16_t e_res2[10]; // reserved2
    int32_t e_lfanew;   // file offset to NT header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
*/

namespace Pepper {

/* DOS header exists at offset 0 into the file.
 */
class DosHeader final : public IHeader {
public:
    enum Fields {
        E_MAGIC,
        E_CBLP,
        E_CP,
        E_CRLC,
        E_CPARHDR,
        E_MINALLOC,
        E_MAXALLOC,
        E_SS,
        E_SP,
        E_CSUM,
        E_IP,
        E_CS,
        E_LFARLC,
        E_OVNO,
        E_RES,
        E_OEMID,
        E_OEMINFO,
        E_RES2,
        E_LFANEW,
        _NUM_FIELDS,
    };

    DosHeader(const FileBytes& fbytes)
    : IHeader(fbytes, 0)
    {
        const int16_t magic = *static_cast<const int16_t*>(hdr());
        if (magic != 0x5A4D && magic != 0x4D5A) {
            throw BadSignature("DOS Header magic is not \"MZ\"");
        }
    }

    // member functions
    const IMAGE_DOS_HEADER* getStructPtr() const { return static_cast<const IMAGE_DOS_HEADER*>(hdr()); }
    const void* getFieldPtr(const int index) const override;

    // static functions
    static const char* getFieldName(const int index);
};
} // namespace Pepper

#endif
