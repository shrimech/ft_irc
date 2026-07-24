#pragma once

#include "Channel.hpp"
#include "context.hpp"

void replyMsg(context& ctx, const std::string& rcode, const std::string& msg);
void joinHandler(context& ctx);
void topicHandler(context &ctx);
