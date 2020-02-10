#ifndef PEFILE_H
#define PEFILE_H

#include <memory>

#include "FileBytes.h"

// headers
#include "headers/DosHdr.h"
#include "headers/FileHdr.h"
#include "headers/OptionalHdr.h"
#include "headers/SectionHdr.h"

// directory entries
#include "headers/ExportDir.h"
#include "headers/ImportDir.h"
#include "headers/ResourceDir.h"
#include "headers/ExceptionDir.h"
#include "headers/CertificateDir.h"
#include "headers/RelocationDir.h"
#include "headers/DebugDir.h"
#include "headers/ArchitectureDir.h"
#include "headers/GlobalPointerDir.h"
#include "headers/TlsDir.h"
#include "headers/LoadConfigDir.h"
#include "headers/BoundImportDir.h"
#include "headers/IatDir.h"
#include "headers/DelayImportDir.h"
#include "headers/ClrDir.h"

namespace Pepper {

class PeFile final {
private:
    std::string      m_fname;
    FileBytes        m_Bytes;

    // headers
    DosHeader        m_Dos;
    FileHeader       m_File;
    OptionalHeader   m_Opt;
    DataDirectory    m_DDir;
    SectionHeaders   m_Sctns;

    // data directories
    ExportDir        m_Xprt;
    ImportDir        m_Mprt;
    ResourceDir      m_Rsrc;
    ExceptionDir     m_Xcpt;
    CertificateDir   m_Cert;
    RelocationDir    m_Reloc;
    DebugDir         m_Dbg;
    ArchitectureDir  m_Rchtr; // unused, all zero
    GlobalPointerDir m_Gptr;  // TODO: find a file with a global pointer
    TlsDir           m_Tls;
    LoadConfigDir    m_Ldcfg;
    BoundImportDir   m_Bmprt;
    IatDir           m_Iat;
    DelayImportDir   m_Dmprt;
    ClrDir           m_Clr;
public:
    enum Headers {
        DOS,
        FILE,
        OPTIONAL,
        DATA_DIR,
        SECTION,
        EXPORT,
        IMPORT,
        RESOURCE,
        EXCEPTION,
        CERTIFICATE,
        RELOCATION,
        DEBUG,
        ARCHITECTURE,
        GLOBAL_POINTER,
        TLS,
        LOAD_CONFIG,
        BOUND_IMPORT,
        IAT,
        DELAY_IMPORT,
        CLR,
        _NUM_HEADERS,
    };

    PeFile(const PeFile &pe) = delete;
    const PeFile& operator=(const PeFile &pe) = delete;

    PeFile(std::string path);

    // member functions
    const DosHeader&        dosHdr()           const { return m_Dos;   }
    const FileHeader&       fileHdr()          const { return m_File;  }
    const OptionalHeader&   optionalHdr()      const { return m_Opt;   }
    const DataDirectory&    dataDir()          const { return m_DDir;  }
    const SectionHeaders&   sectionHdrs()      const { return m_Sctns; }
    const ExportDir&        exportDir()        const { return m_Xprt;  }
    const ImportDir&        importDir()        const { return m_Mprt;  }
    const ResourceDir&      resourceDir()      const { return m_Rsrc;  }
    const ExceptionDir&     exceptionDir()     const { return m_Xcpt;  }
    const CertificateDir&   certificateDir()   const { return m_Cert;  }
    const RelocationDir&    relocationDir()    const { return m_Reloc; }
    const DebugDir&         debugDir()         const { return m_Dbg;   }
    const ArchitectureDir&  architectureDir()  const { return m_Rchtr; }
    const GlobalPointerDir& globalPointerDir() const { return m_Gptr;  }
    const TlsDir&           tlsDir()           const { return m_Tls;   }
    const LoadConfigDir&    loadConfigDir()    const { return m_Ldcfg; }
    const BoundImportDir&   boundImportDir()   const { return m_Bmprt; }
    const IatDir&           iatDir()           const { return m_Iat;   }
    const DelayImportDir&   delayImportDir()   const { return m_Dmprt; }
    const ClrDir&           clrDir()           const { return m_Clr;   }
    void readBytes(const size_t pos, char *buf, const size_t bufLen) const;
    const std::string& path() const { return m_fname; }

    // static functions
    static const char* getHeaderName(const int index);
};
} // namespace Pepper

#endif
