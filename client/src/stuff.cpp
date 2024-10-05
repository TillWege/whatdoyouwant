#include "httplib.h"
#include "stuff.h"

httplib::Client cli("http://127.0.0.1:5479");

httplib::Headers headers = {
	{ "Content-Type", "application/json" }
};

std::string access_token;

bool login(std::string &password)
{
	std::string json_payload = R"({"password": ")" + password + R"("})";

	auto res = cli.Post("/check_password", headers, json_payload, "application/json");

	if(res->status == 200)
	{
		std::cout << "Password correct" << std::endl;
		access_token = password;
		return true;
	} else
	{
		return false;
	}
}

void loadWishes(std::vector<std::string> &wishes)
{
	auto res = cli.Get("/wishes");

	if(res->status == 200)
	{
		std::cout << res->body << std::endl;
		wishes.clear();


		// Remove the surrounding brackets and quotes
		std::string cleanedStr = res->body.substr(1, res->body.length() - 2); // Remove [ and ]
		cleanedStr.erase(std::remove(cleanedStr.begin(), cleanedStr.end(), '\"'), cleanedStr.end());

		// Split the cleaned string by comma
		std::stringstream ss(cleanedStr);
		std::string token;
		while (std::getline(ss, token, ','))
		{
			wishes.push_back(token);
		}
	}
}

void addWish(std::string &wish)
{
	std::string payload = R"({"wish": ")" + std::string(wish) + R"(", "password": ")" + access_token + R"("})";
	auto res = cli.Post("/wishes", headers, payload, "application/json");
	std::cout << res->status << std::endl;
}

void removeWish(int index)
{
	std::string payload = R"({"id": )" + std::to_string(index) + R"(, "password": ")" + access_token + R"("})";
	auto res = cli.Post("/remove", headers, payload, "application/json");
	std::cout << res->status << std::endl;
}

void moveWish(int index, dir direction)
{
	if(direction == UP)
	{
		std::string payload = R"({"id1": )" + std::to_string(index - 1) + R"(, "id2": )" + std::to_string(index) + R"(,"password": ")" + access_token + R"("})";
		std::cout << payload << std::endl;
		auto res = cli.Post("/swap", headers, payload, "application/json");
		std::cout << res->status << std::endl;
		std::cout << res->body << std::endl;
	} else {
		std::string payload = R"({"id1": )" + std::to_string(index) + R"(, "id2": )" + std::to_string(index + 1) + R"(,"password": ")" + access_token + R"("})";
		std::cout << payload << std::endl;
		auto res = cli.Post("/swap", headers, payload, "application/json");
		std::cout << res->status << std::endl;
		std::cout << res->body << std::endl;
	}
}