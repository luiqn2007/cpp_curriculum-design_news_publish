#pragma once

#include <string>
#include <vector>
#include <format>

using std::string;
using std::vector;
using std::stoi;

class Session;
class UserService;
class NewsService;
class CommentService;
class TypeService;

extern Session* session;
extern UserService* user_service;
extern NewsService* news_service;
extern CommentService* comment_service;
extern TypeService* type_service;