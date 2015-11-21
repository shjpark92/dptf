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

#pragma once

#include "Dptf.h"
#include "ParticipantServicesInterface.h"

class XmlNode;

class ControlBase
{
public:

    ControlBase(UIntN participantIndex, UIntN domainIndex, ParticipantServicesInterface* participantServices);
    virtual ~ControlBase();

    virtual void clearCachedData(void) = 0;
    virtual std::string getName(void) = 0;
    virtual XmlNode* getXml(UIntN domainIndex) = 0;

protected:

    UIntN getParticipantIndex() const;
    UIntN getDomainIndex() const;
    ParticipantServicesInterface* getParticipantServices() const;

private:

    UIntN m_participantIndex;
    UIntN m_domainIndex;
    ParticipantServicesInterface* m_participantServices;
};