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

#include "WIParticipantAllocate.h"
#include "ParticipantManager.h"
#include "EsifServices.h"

WIParticipantAllocate::WIParticipantAllocate(DptfManagerInterface* dptfManager, UIntN* newParticipantIndex) :
    WorkItem(dptfManager, FrameworkEvent::ParticipantAllocate),
    m_newParticipantIndex(newParticipantIndex)
{
}

WIParticipantAllocate::~WIParticipantAllocate(void)
{
}

void WIParticipantAllocate::execute(void)
{
    WriteWorkItemStartingInfoMessage();

    try
    {
        getParticipantManager()->allocateParticipant(m_newParticipantIndex);
    }
    catch (std::exception ex)
    {
        *m_newParticipantIndex = Constants::Esif::NoParticipant;
        WriteWorkItemErrorMessage_Function("ParticipantManager::allocateParticipant")
    }
}