#include "data.h"

#include <algorithm>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/filestream.h"

UserMgr usermgr;

std::unordered_map<std::string, User> g_users;

User::User() {
	status = -1;
}

void User::restore() {
	status = -1;
	uid.clear();
	uname.clear();
	psd.clear();
	token.clear();
}

std::string User::Serializable() {
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	d.AddMember("uid", uid.c_str(), allocator);
	d.AddMember("uname", uname.c_str(), allocator);
	d.AddMember("psd", psd.c_str(), allocator);
	d.AddMember("token", token.c_str(), allocator);
	d.AddMember("status", status, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	std::string result = buffer.GetString();

	return result;
}

void User::UnSerializable(const std::string& json) {

}

UserMgr::UserMgr() {

}

bool User::operator ==(const User& user) {
	if (this->psd.compare(user.psd) != 0) return false;
	if (this->uid.compare(user.uid) != 0) return false;
	if (this->uname.compare(user.uname) != 0) return false;
	if (this->token.compare(user.token) != 0) return false;
	if (this->status != user.status) return false;

	return true;
}

int UserMgr::add(const User& user) {

	users.push_back(user);

	return 0;
}

int UserMgr::modify(const User& user) {

	return 0;
}

int UserMgr::remove(const User& user) {

	return 0;
}

int UserMgr::removeWithUid(const std::string& uid) {
	{
		std::lock_guard<std::mutex> lock(m);
		auto v = std::remove_if(users.begin(), users.end(),
				[uid](const User& user) -> bool {
					if (uid.compare(user.uid) == 0) {
						return true;
					}
					return false;
				});
	}
	return 0;
}

int UserMgr::removeWithUname(const std::string& uname) {

	{
		std::lock_guard<std::mutex> lock(m);
		auto v = std::remove_if(users.begin(), users.end(),
				[uname](const User& user) -> bool {
					if (uname.compare(user.uid) == 0) {
						return true;
					}
					return false;
				});
	}

	return 0;
}

User UserMgr::find(const User& user) {
	std::lock_guard<std::mutex> lock(m);
	auto v = std::find_if(users.begin(), users.end(),
			[user](const User& user) -> bool {
				if (user.uid.compare(user.uid) == 0) {
					return true;
				}
				return false;
			});
	if (v == users.end()) {
		return User();
	}
	return (*v);
}

User UserMgr::findWithUid(const std::string& uid) {
	std::lock_guard<std::mutex> lock(m);
	auto v = std::find_if(users.begin(), users.end(),
			[uid](const User& user) -> bool {
				if (uid.compare(user.uid) == 0) {
					return true;
				}
				return false;
			});
	if (v == users.end()) {
		return User();
	}
	return (*v);
}

User UserMgr::findWithUname(const std::string& uname) {
	std::lock_guard<std::mutex> lock(m);
	auto v = std::find_if(users.begin(), users.end(),
			[uname](const User& user) -> bool {
				if (uname.compare(user.uname) == 0) {
					return true;
				}
				return false;
			});
	if (v == users.end()) {
		return User();
	}

	return (*v);
}

bool UserMgr::exist(const std::string& uid) {
	auto v = std::find_if(users.begin(), users.end(),
			[uid](const User& user) -> bool {
				if (uid.compare(user.uid) == 0) {
					return true;
				}
				return false;
			});
	if (v == users.end()) {
		return false;
	}

	return true;
}

bool UserMgr::existWithUname(const std::string& uname) {
	auto v = std::find_if(users.begin(), users.end(),
			[uname](const User& user) -> bool {
				if (uname.compare(user.uname) == 0) {
					return true;
				}
				return false;
			});
	if (v == users.end()) {
		return false;
	}

	return true;

}
