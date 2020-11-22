#include "../include/headers/PeFile.h"

#include <memory>
#include <string>

#include "../include/headers/Types.h"
#include "../include/headers/Exceptions.h"

using namespace Pepper;

PeFile::PeFile(const std::string& path, ExceptionFlag throwFlag)
: m_fname (path)
, m_Bytes (path, throwFlag)
, m_Dos   (m_Bytes, throwFlag)
, m_File  (m_Bytes, m_Dos, throwFlag)
, m_Opt   (m_Bytes, m_File)
, m_DDir  (m_Bytes, m_Opt)
, m_Sctns (m_Bytes, m_File)
{
    m_error = m_Sctns.getError();

    if (m_error == Error::None) {
        if (m_DDir.directories()[DataDirectory::XPRT].rva() > 0 && m_DDir.directories()[DataDirectory::XPRT].size() > 0) {
            m_Xprt = std::make_shared<ExportDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::XPRT]);
        }

        if ((m_DDir.directories()[DataDirectory::MPRT].rva() > 0) && (m_DDir.directories()[DataDirectory::MPRT].size() > 0)) {
            m_Mprt = std::make_shared<ImportDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::MPRT]);
        }

        if ((m_DDir.directories()[DataDirectory::RSRC].rva() > 0) && (m_DDir.directories()[DataDirectory::RSRC].size() > 0)) {
            m_Rsrc = std::make_shared<ResourceDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::RSRC]);
        }

        if ((m_DDir.directories()[DataDirectory::XCPT].rva() > 0) && (m_DDir.directories()[DataDirectory::XCPT].size() > 0)) {
            m_Xcpt = std::make_shared<ExceptionDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::XCPT]);
        }

        if ((m_DDir.directories()[DataDirectory::CERT].rva() > 0) && (m_DDir.directories()[DataDirectory::CERT].size() > 0)) {
            m_Cert = std::make_shared<CertificateDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::CERT]);
        }

        if ((m_DDir.directories()[DataDirectory::RLOC].rva() > 0) && (m_DDir.directories()[DataDirectory::RLOC].size() > 0)) {
            m_Reloc = std::make_shared<RelocationDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::RLOC]);
        }

        if ((m_DDir.directories()[DataDirectory::DBG].rva() > 0) && (m_DDir.directories()[DataDirectory::DBG].size() > 0)) {
            m_Dbg = std::make_shared<DebugDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::DBG]);
        }

        if ((m_DDir.directories()[DataDirectory::ARCH].rva() > 0) && (m_DDir.directories()[DataDirectory::ARCH].size() > 0)) {
            m_Rchtr = std::make_shared<ArchitectureDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::ARCH]);
        }

        if ((m_DDir.directories()[DataDirectory::GLBPTR].rva() > 0) && (m_DDir.directories()[DataDirectory::GLBPTR].size() > 0)) {
            m_Gptr = std::make_shared<GlobalPointerDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::GLBPTR]);
        }

        if ((m_DDir.directories()[DataDirectory::TLS].rva() > 0) && (m_DDir.directories()[DataDirectory::TLS].size() > 0)) {
            m_Tls = std::make_shared<TlsDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::TLS]);
        }

        if ((m_DDir.directories()[DataDirectory::LDCFG].rva() > 0) && (m_DDir.directories()[DataDirectory::LDCFG].size() > 0)) {
            m_Ldcfg = std::make_shared<LoadConfigDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::LDCFG]);
        }

        if ((m_DDir.directories()[DataDirectory::BMPRT].rva() > 0) && (m_DDir.directories()[DataDirectory::BMPRT].size() > 0)) {
            m_Bmprt = std::make_shared<BoundImportDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::BMPRT]);
        }

        if ((m_DDir.directories()[DataDirectory::IAT].rva() > 0) && (m_DDir.directories()[DataDirectory::IAT].size() > 0)) {
            m_Iat = std::make_shared<IatDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::IAT]);
        }

        if ((m_DDir.directories()[DataDirectory::DMPRT].rva() > 0) && (m_DDir.directories()[DataDirectory::DMPRT].size() > 0)) {
            m_Dmprt = std::make_shared<DelayImportDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::DMPRT]);
        }

        if ((m_DDir.directories()[DataDirectory::CLR].rva() > 0) && (m_DDir.directories()[DataDirectory::CLR].size() > 0)) {
            m_Clr = std::make_shared<ClrDir>(*this, m_Bytes, m_DDir.directories()[DataDirectory::CLR]);
        }
    }
}

const char* PeFile::getHeaderName(const int index)
{
    switch (index) {
        case DOS           : return "DOS Header";
        case FILE          : return "File Header";
        case OPTIONAL      : return "Optional Header";
        case DATA_DIR      : return "Data Directory";
        case SECTION       : return "Section Header";
        case EXPORT        : return "Export Directory";
        case IMPORT        : return "Import Directory";
        case RESOURCE      : return "Resource Directory";
        case EXCEPTION     : return "Exception Directory";
        case CERTIFICATE   : return "Certificate Directory";
        case RELOCATION    : return "Relocation Directory";
        case DEBUG         : return "Debug Directory";
        case ARCHITECTURE  : return "Architecture Directory";
        case GLOBAL_POINTER: return "Global Pointer Directory";
        case TLS           : return "TLS Directory";
        case LOAD_CONFIG   : return "Load Config Directory";
        case BOUND_IMPORT  : return "Bound Import Directory";
        case IAT           : return "IAT Directory";
        case DELAY_IMPORT  : return "Delay Import Directory";
        case CLR           : return "CLR Directory";
        default            : return "<UNKNOWN>";
    }
}

void PeFile::readBytes(const offset_t pos, uint8_t* buf, const size_t bufLen) const
{
    m_Bytes.readBytes(pos, buf, bufLen);
}

