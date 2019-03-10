#ifndef VR_SDSOPERATIONPARSER_H
#define VR_SDSOPERATIONPARSER_H

#include "VR_SDSServerDEF.h"
#include "tinyxml.h"

class VR_SDSParserBase
{
public:
    VR_SDSParserBase();
    virtual ~VR_SDSParserBase() {}

    virtual bool ParseResult(const TiXmlElement* pcXmlElem) {
        return true;
    }

    virtual bool HasError() const {
        return m_bHasError;
    }

protected:
    bool m_bHasError;
};


class VR_SDSOperationParser : public VR_SDSParserBase
{
public:
    VR_SDSOperationParser();

    VR_SDSOperationParser(VR_SDSSERVER_OPERATION_TYPE type);

    VR_SDSOperationParser(VR_SDSSERVER_OPERATION_TYPE type, const BL_String& param);

    virtual ~VR_SDSOperationParser() {}

    VR_SDSSERVER_OPERATION_TYPE GetOperationType() const {
        return m_operationType;
    }

    BL_String GetOperationParam() const {
        return m_operationParam;
    }

protected:
    VR_SDSSERVER_OPERATION_TYPE m_operationType;
    BL_String                   m_operationParam;
};

#endif // VR_SDSOPERATIONPARSER_H
