#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <sys/stat.h>
#include <dirent.h>

class FileManagementSystem {
public:
    void createFile(const std::string& filename) {
        std::ofstream file(filename.c_str());
        if (file) {
            std::cout << "File \"" << filename << "\" created successfully." << std::endl;
        } else {
            std::cerr << "Error creating file \"" << filename << "\"." << std::endl;
        }
        file.close();
    }

    void readFile(const std::string& filename) {
        std::ifstream file(filename.c_str());
        if (file) {
            std::string line;
            std::cout << "Content of \"" << filename << "\":" << std::endl;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }
        } else {
            std::cerr << "File \"" << filename << "\" does not exist." << std::endl;
        }
        file.close();
    }

    void writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename.c_str(), std::ios_base::app);
        if (file) {
            file << content << std::endl;
            std::cout << "Content written to \"" << filename << "\"." << std::endl;
        } else {
            std::cerr << "Error writing to file \"" << filename << "\"." << std::endl;
        }
        file.close();
    }

    void deleteFile(const std::string& filename) {
        if (std::remove(filename.c_str()) == 0) {
            std::cout << "File \"" << filename << "\" deleted successfully." << std::endl;
        } else {
            std::cerr << "File \"" << filename << "\" does not exist." << std::endl;
        }
    }

    void renameFile(const std::string& oldName, const std::string& newName) {
        if (std::rename(oldName.c_str(), newName.c_str()) == 0) {
            std::cout << "File renamed from \"" << oldName << "\" to \"" << newName << "\"." << std::endl;
        } else {
            std::cerr << "Error renaming file." << std::endl;
        }
    }

    void getFileInfo(const std::string& filename) {
        struct stat fileStat;
        if (stat(filename.c_str(), &fileStat) == 0) {
            std::cout << "File: " << filename << std::endl;
            std::cout << "Size: " << fileStat.st_size << " bytes" << std::endl;
            std::cout << "Last Modified: " << fileStat.st_mtime << " (epoch time)" << std::endl;
        } else {
            std::cerr << "File \"" << filename << "\" does not exist." << std::endl;
        }
    }

    void listFiles() {
        DIR* dir;
        struct dirent* entry;
        if ((dir = opendir(".")) != NULL) {
            std::cout << "Files in current directory:" << std::endl;
            while ((entry = readdir(dir)) != NULL) {
                std::cout << entry->d_name << std::endl;
            }
            closedir(dir);
        } else {
            std::cerr << "Error opening directory." << std::endl;
        }
    }

    void searchFile(const std::string& pattern) {
        DIR* dir;
        struct dirent* entry;
        if ((dir = opendir(".")) != NULL) {
            std::cout << "Files matching pattern \"" << pattern << "\":" << std::endl;
            while ((entry = readdir(dir)) != NULL) {
                std::string filename = entry->d_name;
                if (filename.find(pattern) != std::string::npos) {
                    std::cout << filename << std::endl;
                }
            }
            closedir(dir);
        } else {
            std::cerr << "Error opening directory." << std::endl;
        }
    }
};

class UserAuthentication {
private:
    std::map<std::string, std::string> users;

public:
    UserAuthentication() {
        users["admin"] = "admin123";
        users["user"] = "user123";
    }

    bool login(std::string& role) {
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (users.find(username) != users.end() && users[username] == password) {
            std::cout << "Login successful!" << std::endl;
            role = username; // Store the role (username)
            return true;
        } else {
            std::cout << "Invalid username or password." << std::endl;
            return false;
        }
    }
};

int main() {
    UserAuthentication auth;
    std::string role;
    if (!auth.login(role)) {
        return 0; 
    }

    FileManagementSystem fms;
    int choice;
    std::string filename, newName, content, searchPattern;

    while (true) {
        std::cout << "\nFile Management System" << std::endl;
        std::cout << "1. Create File" << std::endl;
        std::cout << "2. Read File" << std::endl;
        std::cout << "3. Write to File" << std::endl;
        std::cout << "4. Delete File" << std::endl;
        std::cout << "5. Rename File" << std::endl;
        std::cout << "6. Get File Information" << std::endl;
        std::cout << "7. List Files" << std::endl;
        std::cout << "8. Search File" << std::endl;
        std::cout << "9. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter filename to create: ";
                std::cin >> filename;
                fms.createFile(filename);
                break;
            case 2:
                std::cout << "Enter filename to read: ";
                std::cin >> filename;
                fms.readFile(filename);
                break;
            case 3:
                if (role == "admin") {
                    std::cout << "Enter filename to write to: ";
                    std::cin >> filename;
                    std::cout << "Enter content to write: ";
                    std::cin.ignore();
                    std::getline(std::cin, content);
                    fms.writeFile(filename, content);
                } else {
                    std::cout << "Permission denied. Only admin can write to files." << std::endl;
                }
                break;
            case 4:
                if (role == "admin") {
                    std::cout << "Enter filename to delete: ";
                    std::cin >> filename;
                    fms.deleteFile(filename);
                } else {
                    std::cout << "Permission denied. Only admin can delete files." << std::endl;
                }
                break;
            case 5:
                if (role == "admin") {
                    std::cout << "Enter old filename: ";
                    std::cin >> filename;
                    std::cout << "Enter new filename: ";
                    std::cin >> newName;
                    fms.renameFile(filename, newName);
                } else {
                    std::cout << "Permission denied. Only admin can rename files." << std::endl;
                }
                break;
            case 6:
                if (role == "admin") {
                    std::cout << "Enter filename to get information: ";
                    std::cin >> filename;
                    fms.getFileInfo(filename);
                } else {
                    std::cout << "Permission denied. Only admin can get file information." << std::endl;
                }
                break;
            case 7:
                fms.listFiles();
                break;
            case 8:
                std::cout << "Enter search pattern: ";
                std::cin >> searchPattern;
                fms.searchFile(searchPattern);
                break;
            case 9:
                std::cout << "Exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
                break;
        }
    }

    return 0;
}
