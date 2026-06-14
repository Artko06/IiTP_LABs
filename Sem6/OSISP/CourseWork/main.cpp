#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <fstream>
#include <cstring>
#include <chrono>
#include <dirent.h>
#include <iomanip>
#include <ctime>
#include <pwd.h>
#include <grp.h>

void display_directory(const std::string& path, int selected_index);
void display_tree(const std::string& path, int level);
void handle_input(std::string& current_path, int& selected_index, bool& tree_view_enabled);
char getch();
void view_file(const std::string& file_path);
void copy_item(const std::string& from, const std::string& to);
void move_item(const std::string& from, const std::string& to);
void create_new_directory(const std::string& path);
void create_new_file(const std::string& path);
bool is_directory(const std::string& path);
std::string get_filename(const std::string& path);
std::string get_parent_path(const std::string& path);

bool tree_view_enabled = false;

int main(int argc, char *argv[]) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return 1;
    }

    std::string current_path;
    while (true) {
        std::cout << "Enter path, or press Enter to use current directory [" << cwd << "]: ";
        std::string path_input;
        std::getline(std::cin, path_input);

        current_path = path_input.empty() ? cwd : path_input;
        
        if (is_directory(current_path)) {
            break;
        } else {
            std::cerr << "Error: Invalid path or not a directory: " << current_path << std::endl;
        }
    }

    int selected_index = 0;

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "performance_test") {
            const int num_files = 100000;
            std::cout << "Starting performance test..." << std::endl;

            auto start_create = std::chrono::high_resolution_clock::now();
            std::cout << "Creating test directory 'test_dir'..." << std::endl;
            if (mkdir("test_dir", 0777) != 0 && errno != EEXIST) {
                std::cerr << "Performance test FAILED: could not create directory 'test_dir'" << std::endl;
                return 1;
            }
            std::cout << "Creating " << num_files << " files in 'test_dir'..." << std::endl;
            for (int i = 0; i < num_files; ++i) {
                std::ofstream("test_dir/test_file_" + std::to_string(i));
            }
            auto end_create = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff_create = end_create - start_create;
            std::cout << "File creation finished in " << diff_create.count() << " s" << std::endl;

            std::cout << "Removing test directory 'test_dir' and its contents..." << std::endl;
            auto start_remove = std::chrono::high_resolution_clock::now();
            if (system("rm -rf test_dir") != 0) {
                std::cerr << "Performance test FAILED: cleanup failed." << std::endl;
                return 1;
            }
            auto end_remove = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff_remove = end_remove - start_remove;
            std::cout << "Directory removal finished in " << diff_remove.count() << " s" << std::endl;
            
            std::chrono::duration<double> total_diff = end_remove - start_create;
            std::cout << "Performance test finished in total " << total_diff.count() << " s\n";
            return 0;
        } else if (arg == "correctness_test") {
            std::cout << "Starting correctness test..." << std::endl;
            const char* test_dir = "test_dir";
            
            // 1. Create directory
            std::cout << "Step 1: Creating test directory '" << test_dir << "'..." << std::endl;
            if (mkdir(test_dir, 0777) != 0 && errno != EEXIST) {
                std::cerr << "Correctness test FAILED: could not create directory '" << test_dir << "'" << std::endl;
                return 1;
            }
            std::cout << "        Directory created successfully." << std::endl;

            // 2. Create file with content
            std::string test_filename = std::string(test_dir) + "/test_file.txt";
            std::string test_content = "hello world from correctness test";
            std::cout << "Step 2: Creating test file '" << test_filename << "' with content..." << std::endl;
            std::ofstream test_file(test_filename);
            if (!test_file.is_open()) {
                std::cerr << "Correctness test FAILED: could not create file '" << test_filename << "'" << std::endl;
                system(("rm -rf " + std::string(test_dir)).c_str());
                return 1;
            }
            test_file << test_content;
            test_file.close();
            std::cout << "        File created and content written successfully." << std::endl;

            // 3. Copy file
            std::string copy_filename = std::string(test_dir) + "/test_file_copy.txt";
            std::string copy_command = "cp -r \"" + test_filename + "\" \"" + copy_filename + "\"";
            std::cout << "Step 3: Copying '" << test_filename << "' to '" << copy_filename << "'..." << std::endl;
            if(system(copy_command.c_str()) != 0){
                std::cerr << "Correctness test FAILED: copy command failed." << std::endl;
                system(("rm -rf " + std::string(test_dir)).c_str());
                return 1;
            }
            std::cout << "        File copied successfully." << std::endl;


            // 4. Verify copy
            std::cout << "Step 4: Verifying content of copied file '" << copy_filename << "'..." << std::endl;
            std::ifstream copy_file(copy_filename);
            if (!copy_file.is_open()) {
                std::cerr << "Correctness test FAILED: could not open copied file '" << copy_filename << "'" << std::endl;
                system(("rm -rf " + std::string(test_dir)).c_str());
                return 1;
            }
            std::string copy_content;
            std::getline(copy_file, copy_content);
            copy_file.close();
            if (copy_content != test_content) {
                std::cerr << "Correctness test FAILED: content of copied file is incorrect. Expected '" << test_content << "', got '" << copy_content << "'" << std::endl;
                system(("rm -rf " + std::string(test_dir)).c_str());
                return 1;
            }
            std::cout << "        Copied file content verified successfully." << std::endl;
            
            // 5. Move file
            std::string moved_filename = std::string(test_dir) + "/test_file_moved.txt";
            std::cout << "Step 5: Moving '" << test_filename << "' to '" << moved_filename << "'..." << std::endl;
            if (rename(test_filename.c_str(), moved_filename.c_str()) != 0) {
                perror("rename");
                std::cerr << "Correctness test FAILED: move operation failed." << std::endl;
                system(("rm -rf " + std::string(test_dir)).c_str());
                return 1;
            }
            std::cout << "        File moved successfully." << std::endl;

            // 6. Verify move - check if new file exists and old file is gone
            std::cout << "Step 6: Verifying move operation..." << std::endl;
            std::ifstream moved_file_check(moved_filename);
            if(!moved_file_check.is_open()){
                std::cerr << "Correctness test FAILED: move operation failed, moved file not found." << std::endl;
                system(("rm -rf " + std::string(test_dir)).c_str());
                return 1;
            }
            moved_file_check.close();
            std::cout << "        New file '" << moved_filename << "' found." << std::endl;
            
            std::ifstream old_file_check(test_filename);
            if(old_file_check.is_open()){
                std::cerr << "Correctness test FAILED: old file '" << test_filename << "' still exists after move." << std::endl;
                system(("rm -rf " + std::string(test_dir)).c_str());
                return 1;
            }
            std::cout << "        Original file '" << test_filename << "' successfully removed." << std::endl;

            // 7. Cleanup
            std::cout << "Step 7: Cleaning up test directory '" << test_dir << "'..." << std::endl;
            if (system(("rm -rf " + std::string(test_dir)).c_str()) != 0) {
                std::cerr << "Correctness test FAILED: cleanup failed." << std::endl;
                return 1;
            }
            std::cout << "        Cleanup successful." << std::endl;

            std::cout << "Correctness test PASSED." << std::endl;
            return 0;
        }
    }

    while (true) {
        if (tree_view_enabled) {
            system("clear");
            std::cout << "Current directory: " << current_path << std::endl;
            std::cout << "Controls: (t) - toggle tree view, (b) - back, (q) - quit" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            display_tree(current_path, 0);
        } else {
            display_directory(current_path, selected_index);
        }
        handle_input(current_path, selected_index, tree_view_enabled);
    }

    return 0;
}

char getch() {
    char buf = 0;
    struct termios old;
    memset(&old, 0, sizeof(old));
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

bool is_directory(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

std::string get_filename(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

std::string get_parent_path(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos && pos > 0) {
        return path.substr(0, pos);
    }
    if (pos == 0 && path.length() > 1) {
        return "/";
    }
    return "";
}


void display_tree(const std::string& path, int level) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path.c_str()))) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        for (int i = 0; i < level; ++i) {
            std::cout << "  ";
        }
        std::cout << "|-- " << entry->d_name << std::endl;

        std::string entry_path = path + "/" + entry->d_name;
        if (is_directory(entry_path)) {
            display_tree(entry_path, level + 1);
        }
    }
    closedir(dir);
}

void display_directory(const std::string& path, int selected_index) {
    system("clear");
    std::cout << "Current directory: " << path << std::endl;
    std::cout << "Controls: (w/s) - up/down, (f) - find, (enter) - open, (b) - back, (c) - copy, (m) - move, (d) - delete, (n) - new file, (N) - new directory, (t) - tree view, (q) - quit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::vector<std::string> entries;
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path.c_str()))) {
        perror("opendir");
        getch();
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            entries.push_back(entry->d_name);
        }
    }
    closedir(dir);
    std::sort(entries.begin(), entries.end());


    for (size_t i = 0; i < entries.size(); ++i) {
        if (i == static_cast<size_t>(selected_index)) {
            std::cout << "> ";
        } else {
            std::cout << "  ";
        }
        std::cout << entries[i] << std::endl;
    }
}

void handle_input(std::string& current_path, int& selected_index, bool& tree_view_enabled) {
    char c = getch();
    std::vector<std::string> entries;
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(current_path.c_str()))) {
        perror("opendir");
        getch();
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            entries.push_back(entry->d_name);
        }
    }
    closedir(dir);
    std::sort(entries.begin(), entries.end());


    switch (c) {
        case 't': // Tree
            tree_view_enabled = !tree_view_enabled;
            break;
        case 'w': // Up
            if (!tree_view_enabled && selected_index > 0) {
                selected_index--;
            }
            break;
        case 's': // Down
            if (!tree_view_enabled && selected_index < static_cast<int>(entries.size()) - 1) {
                selected_index++;
            }
            break;
        case 'f': // Find
            if (!tree_view_enabled) {
                std::cout << "Enter name to find: ";
                std::string name_to_find;
                std::cin >> name_to_find;

                auto it = std::find(entries.begin(), entries.end(), name_to_find);
                if (it != entries.end()) {
                    selected_index = std::distance(entries.begin(), it);
                } else {
                    std::cout << "\n'" << name_to_find << "' not found. Press any key to continue." << std::flush;
                    getch();
                }
            }
            break;
        case '\n': // Enter
            if (!tree_view_enabled && !entries.empty()) {
                std::string selected_path = current_path + "/" + entries[selected_index];
                if (is_directory(selected_path)) {
                    current_path = selected_path;
                    selected_index = 0;
                } else {
                    view_file(selected_path);
                }
            }
            break;
        case 'b': // Back
        {
            std::string parent_path = get_parent_path(current_path);
            if (!parent_path.empty() && parent_path != current_path) {
                current_path = parent_path;
                selected_index = 0;
            } else if (current_path != "/") {
                 current_path = "/";
                 selected_index = 0;
            }
        }
            break;
        case 'c': // Copy
            if (!tree_view_enabled && !entries.empty()) {
                std::string from = current_path + "/" + entries[selected_index];
                std::cout << "Enter destination path: ";
                std::string to_str;
                std::cin >> to_str;
                
                std::string final_to_path = to_str;
                if (!to_str.empty() && to_str[0] != '/') {
                    final_to_path = current_path + "/" + to_str;
                }
                copy_item(from, final_to_path);
            }
            break;
        case 'm': // Move
            if (!tree_view_enabled && !entries.empty()) {
                std::string from = current_path + "/" + entries[selected_index];
                std::cout << "Enter destination path: ";
                std::string to_str;
                std::cin >> to_str;

                std::string final_to_path = to_str;
                if (!to_str.empty() && to_str[0] != '/') {
                    final_to_path = current_path + "/" + to_str;
                }
                move_item(from, final_to_path);
            }
            break;
        case 'd': // Delete
            if (!tree_view_enabled && !entries.empty()) {
                std::string path_to_delete = current_path + "/" + entries[selected_index];
                std::cout << "Are you sure you want to delete " << get_filename(path_to_delete) << "? (y/n): ";
                std::cout << std::flush;
                char choice = getch();
                if (choice == 'y') {
                    if (is_directory(path_to_delete)) {
                        std::string command = "rm -rf \"" + path_to_delete + "\"";
                        system(command.c_str());
                    } else {
                        remove(path_to_delete.c_str());
                    }
                    if (selected_index > 0 && selected_index >= static_cast<int>(entries.size()) - 1) {
                        selected_index--;
                    }
                }
            }
            break;
        case 'n':
            if (!tree_view_enabled) {
                std::cout << "Enter new file name: ";
                std::string name;
                std::cin >> name;
                create_new_file(current_path + "/" + name);
            }
            break;
        case 'N':
            if (!tree_view_enabled) {
                std::cout << "Enter new directory name: ";
                std::string name;
                std::cin >> name;
                create_new_directory(current_path + "/" + name);
            }
            break;
        case 'q':
            exit(0);
            break;
    }
}

void view_file(const std::string& file_path) {
    system("clear");

    struct stat st;
    if (stat(file_path.c_str(), &st) == 0) {
        std::cout << "--- Metadata for " << get_filename(file_path) << " ---" << std::endl;
        
        std::cout << "Permissions: ";
        std::cout << (S_ISDIR(st.st_mode) ? 'd' : '-');
        std::cout << ((st.st_mode & S_IRUSR) ? 'r' : '-');
        std::cout << ((st.st_mode & S_IWUSR) ? 'w' : '-');
        std::cout << ((st.st_mode & S_IXUSR) ? 'x' : '-');
        std::cout << ((st.st_mode & S_IRGRP) ? 'r' : '-');
        std::cout << ((st.st_mode & S_IWGRP) ? 'w' : '-');
        std::cout << ((st.st_mode & S_IXGRP) ? 'x' : '-');
        std::cout << ((st.st_mode & S_IROTH) ? 'r' : '-');
        std::cout << ((st.st_mode & S_IWOTH) ? 'w' : '-');
        std::cout << ((st.st_mode & S_IXOTH) ? 'x' : '-') << std::endl;
        
        struct passwd *pw = getpwuid(st.st_uid);
        struct group  *gr = getgrgid(st.st_gid);
        if (pw) std::cout << "Owner: " << pw->pw_name << std::endl;
        if (gr) std::cout << "Group: " << gr->gr_name << std::endl;

        std::cout << "Size: " << st.st_size << " bytes" << std::endl;

        char time_buf[80];
        time_t mtime = st.st_mtime;
        strftime(time_buf, sizeof(time_buf), "%c", localtime(&mtime));
        std::cout << "Last modified: " << time_buf << std::endl;

        std::cout << "------------------------------------" << std::endl << std::endl;
    }

    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
        std::cout << "\nPress any key to continue...";
        getch();
    } else {
        perror("open");
        std::cout << "\nPress any key to continue...";
        getch();
    }
}

void copy_item(const std::string& from, const std::string& to) {
    std::string command = "cp -r \"" + from + "\" \"" + to + "\"";
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error copying item. Command failed: " << command << std::endl;
        std::cout << "\nPress any key to continue...";
        getch();
    }
}

void move_item(const std::string& from, const std::string& to) {
    std::string final_to_path = to;
    if (is_directory(to)) {
        final_to_path = to + "/" + get_filename(from);
    }

    if (rename(from.c_str(), final_to_path.c_str()) != 0) {
        perror("rename");
        std::cout << "\nPress any key to continue...";
        getch();
    }
}

void create_new_directory(const std::string& path) {
    if (mkdir(path.c_str(), 0777) != 0) {
        perror("mkdir");
        std::cout << "\nPress any key to continue...";
        getch();
    }
}

void create_new_file(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        perror("create file");
        std::cout << "\nPress any key to continue...";
        getch();
    }
}
