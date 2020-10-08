#ifndef PEFILE_H
#define PEFILE_H

#include <memory>

#include "FileBytes.h"

#include "pe_structures/DosHdr.h"
#include "pe_structures/FileHdr.h"
#include "pe_structures/OptionalHdr.h"
#include "pe_structures/SectionHdr.h"

#include "pe_structures/ExportDir.h"
#include "pe_structures/ImportDir.h"
#include "pe_structures/ResourceDir.h"
#include "pe_structures/ExceptionDir.h"
#include "pe_structures/CertificateDir.h"
#include "pe_structures/RelocationDir.h"
#include "pe_structures/DebugDir.h"
#include "pe_structures/ArchitectureDir.h"
#include "pe_structures/GlobalPointerDir.h"
#include "pe_structures/TlsDir.h"
#include "pe_structures/LoadConfigDir.h"
#include "pe_structures/BoundImportDir.h"
#include "pe_structures/IatDir.h"
#include "pe_structures/DelayImportDir.h"
#include "pe_structures/ClrDir.h"

namespace Pepper {

using ExportDirPtr        = std::shared_ptr<ExportDir>;
using ImportDirPtr        = std::shared_ptr<ImportDir>;
using ResourceDirPtr      = std::shared_ptr<ResourceDir>;
using ExceptionDirPtr     = std::shared_ptr<ExceptionDir>;
using CertificateDirPtr   = std::shared_ptr<CertificateDir>;
using RelocationDirPtr    = std::shared_ptr<RelocationDir>;
using DebugDirPtr         = std::shared_ptr<DebugDir>;
using ArchitectureDirPtr  = std::shared_ptr<ArchitectureDir>;
using GlobalPointerDirPtr = std::shared_ptr<GlobalPointerDir>;
using TlsDirPtr           = std::shared_ptr<TlsDir>;
using LoadConfigDirPtr    = std::shared_ptr<LoadConfigDir>;
using BoundImportDirPtr   = std::shared_ptr<BoundImportDir>;
using IatDirPtr           = std::shared_ptr<IatDir>;
using DelayImportDirPtr   = std::shared_ptr<DelayImportDir>;
using ClrDirPtr           = std::shared_ptr<ClrDir>;

class PeFile final {
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

    PeFile(const PeFile& pe) = delete;
    PeFile& operator=(const PeFile& pe) = delete;

    PeFile(const std::string& path);

    const DosHeader&           dosHdr()           const { return m_Dos;   }
    const FileHeader&          fileHdr()          const { return m_File;  }
    const OptionalHeader&      optionalHdr()      const { return m_Opt;   }
    const DataDirectory&       dataDir()          const { return m_DDir;  }
    const SectionHeaders&      sectionHdrs()      const { return m_Sctns; }
    const ExportDirPtr&        exportDir()        const { return m_Xprt;  }
    const ImportDirPtr&        importDir()        const { return m_Mprt;  }
    const ResourceDirPtr&      resourceDir()      const { return m_Rsrc;  }
    const ExceptionDirPtr&     exceptionDir()     const { return m_Xcpt;  }
    const CertificateDirPtr&   certificateDir()   const { return m_Cert;  }
    const RelocationDirPtr&    relocationDir()    const { return m_Reloc; }
    const DebugDirPtr&         debugDir()         const { return m_Dbg;   }
    const ArchitectureDirPtr&  architectureDir()  const { return m_Rchtr; }
    const GlobalPointerDirPtr& globalPointerDir() const { return m_Gptr;  }
    const TlsDirPtr&           tlsDir()           const { return m_Tls;   }
    const LoadConfigDirPtr&    loadConfigDir()    const { return m_Ldcfg; }
    const BoundImportDirPtr&   boundImportDir()   const { return m_Bmprt; }
    const IatDirPtr&           iatDir()           const { return m_Iat;   }
    const DelayImportDirPtr&   delayImportDir()   const { return m_Dmprt; }
    const ClrDirPtr&           clrDir()           const { return m_Clr;   }
    void readBytes(const offset_t pos, uint8_t* buf, const size_t bufLen) const;
    const std::string& path() const { return m_fname; }

    static const char* getHeaderName(const int index);

private:
    std::string      m_fname;
    FileBytes        m_Bytes;

    DosHeader        m_Dos;
    FileHeader       m_File;
    OptionalHeader   m_Opt;
    DataDirectory    m_DDir;
    SectionHeaders   m_Sctns;

    ExportDirPtr        m_Xprt{};
    ImportDirPtr        m_Mprt{};
    ResourceDirPtr      m_Rsrc{};
    ExceptionDirPtr     m_Xcpt{};
    CertificateDirPtr   m_Cert{};
    RelocationDirPtr    m_Reloc{};
    DebugDirPtr         m_Dbg{};
    ArchitectureDirPtr  m_Rchtr{}; // unused, all zero
    GlobalPointerDirPtr m_Gptr{};  // TODO: find a file with a global pointer
    TlsDirPtr           m_Tls{};
    LoadConfigDirPtr    m_Ldcfg{};
    BoundImportDirPtr   m_Bmprt{};
    IatDirPtr           m_Iat{};
    DelayImportDirPtr   m_Dmprt{};
    ClrDirPtr           m_Clr{};
};
} // namespace Pepper

#endif
