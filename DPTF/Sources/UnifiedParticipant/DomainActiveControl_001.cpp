/******************************************************************************
** Copyright (c) 2013-2015 Intel Corporation All Rights Reserved
**
** Licensed under the Apache License, Version 2.0 (the "License"); you may not
** use this file except in compliance with the License.
**
** You may obtain a copy of the License at
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
** WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**
** See the License for the specific language governing permissions and
** limitations under the License.
**
******************************************************************************/

#include "DomainActiveControl_001.h"
#include "XmlNode.h"

DomainActiveControl_001::DomainActiveControl_001(UIntN participantIndex, UIntN domainIndex, 
    ParticipantServicesInterface* participantServicesInterface) :
    DomainActiveControlBase(participantIndex, domainIndex, participantServicesInterface),
    m_activeControlStaticCaps(nullptr),
    m_activeControlStatus(nullptr),
    m_activeControlSet(nullptr)
{
}

DomainActiveControl_001::~DomainActiveControl_001(void)
{
    clearCachedData();
}

ActiveControlStaticCaps DomainActiveControl_001::getActiveControlStaticCaps(UIntN participantIndex, UIntN domainIndex)
{
    checkAndCreateControlStructures(domainIndex);

    return *m_activeControlStaticCaps;
}

ActiveControlStatus DomainActiveControl_001::getActiveControlStatus(UIntN participantIndex, UIntN domainIndex)
{
    checkAndCreateControlStructures(domainIndex);

    return *m_activeControlStatus;
}

ActiveControlSet DomainActiveControl_001::getActiveControlSet(UIntN participantIndex, UIntN domainIndex)
{
    checkAndCreateControlStructures(domainIndex);

    return *m_activeControlSet;
}

void DomainActiveControl_001::setActiveControl(UIntN participantIndex, UIntN domainIndex, UIntN controlIndex)
{
    checkAndCreateControlStructures(domainIndex);

    if (m_activeControlStaticCaps->supportsFineGrainedControl())
    {
        // If fine grain control is supported, use the method that takes a Percentage as the set parameter
        throw dptf_exception("Wrong function called.  Since fine grain control is supported, use the \
            setActiveControl function that take a Percentage as an argument.");
    }

    // Verify that control Id is valid
    if (controlIndex >= m_activeControlSet->getCount())
    {
        throw dptf_exception("The desired control index is out of bounds.");
    }

    getParticipantServices()->primitiveExecuteSetAsUInt32(
        esif_primitive_type::SET_FAN_LEVEL,
        (*m_activeControlSet)[controlIndex].getControlId(),
        domainIndex);

    DELETE_MEMORY_TC(m_activeControlStatus);

    createActiveControlStatus(domainIndex);
}

void DomainActiveControl_001::setActiveControl(UIntN participantIndex, UIntN domainIndex, const Percentage& fanSpeed)
{
    checkAndCreateControlStructures(domainIndex);

    if (!m_activeControlStaticCaps->supportsFineGrainedControl())
    {
        // If fine grain control is supported, use the method that takes a Percentage as the set parameter
        throw dptf_exception("Wrong function called.  Since fine grain control is not supported, use the \
                             setActiveControl function that take a control index as an argument.");
    }

    // FIXME: For now SET_FAN_LEVEL doesn't follow the normal rule for percentages.  For this we pass in a whole number
    // which would be 90 for 90%.  This is an exception and should be corrected in the future.
    UInt32 convertedFanSpeedPercentage = static_cast<UInt32>(fanSpeed * 100);

    getParticipantServices()->primitiveExecuteSetAsUInt32(
        esif_primitive_type::SET_FAN_LEVEL,
        convertedFanSpeedPercentage,
        domainIndex);

    DELETE_MEMORY_TC(m_activeControlStatus);

    createActiveControlStatus(domainIndex);
}

void DomainActiveControl_001::clearCachedData(void)
{
    DELETE_MEMORY_TC(m_activeControlSet);
    DELETE_MEMORY_TC(m_activeControlStaticCaps);
    DELETE_MEMORY_TC(m_activeControlStatus);
}

XmlNode* DomainActiveControl_001::getXml(UIntN domainIndex)
{
    checkAndCreateControlStructures(domainIndex);

    XmlNode* root = XmlNode::createWrapperElement("active_control");
    root->addChild(m_activeControlStatus->getXml());
    root->addChild(m_activeControlStaticCaps->getXml());
    root->addChild(m_activeControlSet->getXml());
    root->addChild(XmlNode::createDataElement("control_knob_version", "001"));

    return root;
}

void DomainActiveControl_001::createActiveControlStaticCaps(UIntN domainIndex)
{
    DptfBuffer buffer = getParticipantServices()->primitiveExecuteGet(
        esif_primitive_type::GET_FAN_INFORMATION, ESIF_DATA_BINARY, domainIndex);
    m_activeControlStaticCaps = BinaryParse::fanFifObject(buffer);
}

void DomainActiveControl_001::createActiveControlStatus(UIntN domainIndex)
{
    DptfBuffer buffer = getParticipantServices()->primitiveExecuteGet(
        esif_primitive_type::GET_FAN_STATUS, ESIF_DATA_BINARY, domainIndex);
    m_activeControlStatus = BinaryParse::fanFstObject(buffer);
}

void DomainActiveControl_001::createActiveControlSet(UIntN domainIndex)
{
    DptfBuffer buffer = getParticipantServices()->primitiveExecuteGet(
        esif_primitive_type::GET_FAN_PERFORMANCE_STATES, ESIF_DATA_BINARY, domainIndex);
    m_activeControlSet = new ActiveControlSet(BinaryParse::fanFpsObject(buffer));
}

void DomainActiveControl_001::checkAndCreateControlStructures(UIntN domainIndex)
{
    if (m_activeControlStaticCaps == nullptr)
    {
        createActiveControlStaticCaps(domainIndex);
    }

    if (m_activeControlStatus == nullptr)
    {
        createActiveControlStatus(domainIndex);
    }

    if (m_activeControlSet == nullptr)
    {
        createActiveControlSet(domainIndex);
    }
}

std::string DomainActiveControl_001::getName(void)
{
    return "Active Control (Version 1)";
}
