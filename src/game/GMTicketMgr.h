/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _GMTICKETMGR_H
#define _GMTICKETMGR_H

#include "Policies/Singleton.h"
#include "Database/DatabaseEnv.h"
#include "Util.h"
#include "ObjectGuid.h"
#include <map>

class GMTicket
{
    public:
        explicit GMTicket() : m_lastUpdate(0)
        {
        }

        void Init(ObjectGuid guid, const std::string& text, const std::string& responsetext, time_t update, uint8 closed)
        {
            m_guid = guid;
            m_text = text;
            m_responseText = responsetext;
            m_lastUpdate =update;
            m_closed = closed;
        }

        ObjectGuid const& GetPlayerGuid() const
        {
            return m_guid;
        }

        const char* GetText() const
        {
            return m_text.c_str();
        }

        const char* GetResponse() const
        {
            return m_responseText.c_str();
        }

        uint64 GetLastUpdate() const
        {
            return m_lastUpdate;
        }

        void SetText(const char* text)
        {
            m_text = text ? text : "";
            m_lastUpdate = time(NULL);

            std::string escapedString = m_text;
            CharacterDatabase.escape_string(escapedString);
            CharacterDatabase.PExecute("UPDATE character_ticket SET ticket_text = '%s' WHERE (guid = '%u' AND closed = '0')", escapedString.c_str(), m_guid.GetCounter());
        }

// patch professional ticket response
//      void SetResponseText(const char* text)
        void SetResponseText(const char* text, uint32 who_answer)
//
        {
            m_responseText = text ? text : "";
            m_lastUpdate = time(NULL);

            std::string escapedString = m_responseText;
            CharacterDatabase.escape_string(escapedString);
// patch professional ticket response
//          CharacterDatabase.PExecute("UPDATE character_ticket SET response_text = '%s' WHERE (guid = '%u' AND closed = '0')", escapedString.c_str(), m_guid.GetCounter());
            CharacterDatabase.PExecute("UPDATE character_ticket SET response_text = '%s', response_date = NOW(), response_who = '%u' WHERE (guid = '%u' AND closed = '0')", escapedString.c_str(), who_answer, m_guid.GetCounter());
//
        }

        bool HasResponse() { return !m_responseText.empty(); }

        void CloseInDB() const
        {
            CharacterDatabase.PExecute("UPDATE character_ticket SET closed = '1' WHERE (guid = '%u' AND closed = '0') LIMIT 1", m_guid.GetCounter());
        }

        void SaveToDB() const
        {
            CharacterDatabase.BeginTransaction();
            CloseInDB();

            std::string escapedString = m_text;
            CharacterDatabase.escape_string(escapedString);

            std::string escapedString2 = m_responseText;
            CharacterDatabase.escape_string(escapedString2);

            CharacterDatabase.PExecute("INSERT INTO character_ticket (guid, ticket_text, response_text, closed) VALUES ('%u', '%s', '%s', '0')", m_guid.GetCounter(), escapedString.c_str(), escapedString2.c_str());
            CharacterDatabase.CommitTransaction();
        }
    private:
        ObjectGuid m_guid;
        std::string m_text;
        std::string m_responseText;
        time_t m_lastUpdate;
        uint8 m_closed;
};
typedef std::map<ObjectGuid, GMTicket> GMTicketMap;
typedef std::list<GMTicket*> GMTicketList;                  // for creating order access

class GMTicketMgr
{
    public:
        GMTicketMgr() {  }
        ~GMTicketMgr() {  }

        void LoadGMTickets();

        GMTicket* GetGMTicket(ObjectGuid guid)
        {
            GMTicketMap::iterator itr = m_GMTicketMap.find(guid);
            if(itr == m_GMTicketMap.end())
                return NULL;
            return &(itr->second);
        }

        size_t GetTicketCount() const
        {
            return m_GMTicketMap.size();
        }

        GMTicket* GetGMTicketByOrderPos(uint32 pos)
        {
            if (pos >= GetTicketCount())
                return NULL;

            GMTicketList::iterator itr = m_GMTicketListByCreatingOrder.begin();
            std::advance(itr, pos);
            if(itr == m_GMTicketListByCreatingOrder.end())
                return NULL;
            return *itr;
        }


        void Close(ObjectGuid guid)
        {
            GMTicketMap::iterator itr = m_GMTicketMap.find(guid);
            if(itr == m_GMTicketMap.end())
                return;
            itr->second.CloseInDB();
            m_GMTicketListByCreatingOrder.remove(&itr->second);
            m_GMTicketMap.erase(itr);
        }

        void CloseAll();

        void Create(ObjectGuid guid, const char* text)
        {
            GMTicket& ticket = m_GMTicketMap[guid];
            if (ticket.GetPlayerGuid())                     // overwrite ticket
            {
                ticket.CloseInDB();
                m_GMTicketListByCreatingOrder.remove(&ticket);
            }

            ticket.Init(guid, text, "", time(NULL), 0);
            ticket.SaveToDB();
            m_GMTicketListByCreatingOrder.push_back(&ticket);
        }
    private:
        GMTicketMap m_GMTicketMap;
        GMTicketList m_GMTicketListByCreatingOrder;
};

#define sTicketMgr MaNGOS::Singleton<GMTicketMgr>::Instance()
#endif
