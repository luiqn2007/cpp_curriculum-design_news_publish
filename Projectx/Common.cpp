#include "Common.h"

#include "Session.h"
#include "UserService.h"
#include "NewsService.h"
#include "CommentService.h"
#include "TypeService.h"

Session* session = new Session;
UserService* user_service = new UserService;
NewsService* news_service = new NewsService;
CommentService* comment_service = new CommentService;
TypeService* type_service = new TypeService;