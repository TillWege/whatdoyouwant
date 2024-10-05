#include <httplib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

std::string global_password;
std::vector<std::string> wishes;

std::string generate_random_password(size_t length) {
	const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	std::default_random_engine rng(std::random_device{}());
	std::uniform_int_distribution<size_t> dist(0, sizeof(charset) - 2);

	std::string password;
	for (size_t i = 0; i < length; ++i) {
		password += charset[dist(rng)];
	}
	return password;
}

void initialize_password() {
	const std::string filename = "login.txt";

	if (std::filesystem::exists(filename)) {
		std::ifstream file(filename);
		if (file.is_open()) {
			std::getline(file, global_password);
			file.close();
		}
	} else {
		global_password = generate_random_password(10);
		std::ofstream file(filename);
		if (file.is_open()) {
			file << global_password;
			file.close();
		}
	}
	std::cout << "Password: " << global_password << std::endl;
}

void initialize_wishes() {
	const std::string filename = "wishes.txt";

	if (std::filesystem::exists(filename)) {
		std::ifstream file(filename);
		if (file.is_open()) {
			std::string wish;
			while (std::getline(file, wish)) {
				wishes.push_back(wish);
			}
			file.close();
		}
	} else {
		std::ofstream file(filename);
		if (file.is_open()) {
			file.close();
		}
	}
}

void save_wishes() {
	const std::string filename = "wishes.txt";
	std::ofstream file(filename);
	if (file.is_open()) {
		for (const auto& wish : wishes) {
			file << wish << "\n";
		}
		file.close();
	}
}

int main() {
	initialize_password();
	initialize_wishes();

	httplib::Server server;

	server.Post("/wishes", [](const httplib::Request &req, httplib::Response &res) {
		std::cout << "POST Request: " << req.path << std::endl;
		try {
			auto body_json = json::parse(req.body);
			if (body_json.contains("password") && body_json["password"] == global_password) {
				if (body_json.contains("wish")) {
					wishes.push_back(body_json["wish"]);
					save_wishes();
					res.set_content(R"({"Message": "Wish added successfully"})", "application/json");
				} else {
					res.set_content(R"({"Message": "OK"})", "application/json");
				}
			} else {
				res.status = 401;
				res.set_content(R"({"error": "Unauthorized"})", "application/json");
			}
		} catch (const json::parse_error&) {
			res.status = 400;
			res.set_content(R"({"error": "Bad Request"})", "application/json");
		}
	});

	server.Post("/check_password", [](const httplib::Request &req, httplib::Response &res) {
		std::cout << "POST Request: " << req.path << std::endl;
		try {
			auto body_json = json::parse(req.body);
			if (body_json.contains("password") && body_json["password"] == global_password) {
				res.set_content(R"({"Message": "Password is correct"})", "application/json");
			} else {
				res.status = 401;
				res.set_content(R"({"error": "Unauthorized"})", "application/json");
			}
		} catch (const json::parse_error&) {
			res.status = 400;
			res.set_content(R"({"error": "Bad Request"})", "application/json");
		}
	});

	server.Post("/swap", [](const httplib::Request &req, httplib::Response &res) {
		std::cout << "POST Request: " << req.path << std::endl;
		try {
			auto body_json = json::parse(req.body);
			if (body_json.contains("password") && body_json["password"] == global_password) {
				if (body_json.contains("id1") && body_json.contains("id2")) {
					int id1 = body_json["id1"].get<int>();
					int id2 = body_json["id2"].get<int>();
					if (id1 >= 0 && id1 < wishes.size() && id2 >= 0 && id2 < wishes.size()) {
						std::swap(wishes[id1], wishes[id2]);
						save_wishes();
						res.set_content(R"({"Message": "Wishes swapped successfully"})", "application/json");
					} else {
						res.status = 400;
						res.set_content(R"({"error": "Invalid wish IDs"})", "application/json");
					}
				} else {
					res.status = 400;
					res.set_content(R"({"error": "Missing wish IDs"})", "application/json");
				}
			} else {
				res.status = 401;
				res.set_content(R"({"error": "Unauthorized"})", "application/json");
			}
		} catch (const json::parse_error&) {
			res.status = 400;
			res.set_content(R"({"error": "Bad Request"})", "application/json");
		}
	});

	server.Post("/remove", [](const httplib::Request &req, httplib::Response &res) {
		std::cout << "POST Request: " << req.path << std::endl;
		try {
			auto body_json = json::parse(req.body);
			if (body_json.contains("password") && body_json["password"] == global_password) {
				if (body_json.contains("id")) {
					int id = body_json["id"].get<int>();
					if (id >= 0 && id < wishes.size()) {
						wishes.erase(wishes.begin() + id);
						save_wishes();
						res.set_content(R"({"Message": "Wish removed successfully"})", "application/json");
					} else {
						res.status = 400;
						res.set_content(R"({"error": "Invalid wish ID"})", "application/json");
					}
				} else {
					res.status = 400;
					res.set_content(R"({"error": "Missing wish ID"})", "application/json");
				}
			} else {
				res.status = 401;
				res.set_content(R"({"error": "Unauthorized"})", "application/json");
			}
		} catch (const json::parse_error&) {
			res.status = 400;
			res.set_content(R"({"error": "Bad Request"})", "application/json");
		}
	});

	server.Post(".*", [](const httplib::Request &req, httplib::Response &res) {
		res.status = 404;
		res.set_content(R"({"error": "Not Found"})", "application/json");
	});

	server.Get("/wishes", [](const httplib::Request &req, httplib::Response &res) {
		std::cout << "GET Request: " << req.path << std::endl;
		json response_json = wishes;
		res.set_content(response_json.dump(), "application/json");
	});

	std::cout << "Starting server on port 5479..." << std::endl;
	server.listen("0.0.0.0", 5479);

	return 0;
}
