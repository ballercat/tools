#include <iostream>
#include "../args.h"
#include "../basics.h"
#include <fstream>
#include <sstream>
#include <locale>

using namespace std;

struct ClassDef {
  std::string name;
  std::string ctor_str;

  bool ctor = false;   /// -> constructor
  bool dctor = false;  /// -> default constructor
  bool dtor = false;   /// -> destructor
  bool ddtor = false;  /// -> default destructor
  bool cctor = false;  /// -> copy constructor
  bool dcctor = false; /// -> default copy constructor
  bool mctor = false;  /// -> move constructor
  bool dmctor = false; /// -> default move constructor

  bool del_move = false;
  bool del_copy = false;

  /// Operators
  bool asign = false;
  bool c_asign = false;
  bool dc_asign = false;
  bool m_asign = false;
  bool dm_asign = false;
  bool masign = false;
  bool equal = false;
  bool less = false;
};

struct FileDef {
  std::string license;
  std::string path;
  std::string name;
  std::string header_guard;

  bool insert_license = false;

  ClassDef cdef;
};

void insert_license(std::ostream &out, const std::string &l) {
  out << l << std::endl;
}

void insert_doxygen_full(std::ostream &out) {
  out << "///\n";
  out << "///\n";
  out << "/// \\brief\n";
  out << "///\n";
  out << "/// Typical usage:\n";
  out << "/// \\code\n";
  out << "///\n";
  out << "/// \\param\n";
  out << "/// \\returns\n";
}

void insert_doxygen_quick(std::ostream &out) { out << "/// \brief\n"; }

void write_asign_operator(std::ostream &out, const ClassDef &c) {
  out << "     " << c.name << "& operator=(const " << c.name << "& copy)";
  if (c.c_asign)
    out << " {\n\n     return *this;\n     }\n";
  else if (c.del_copy)
    out << " = delete;\n";
  else
    out << " = default;\n";

  out << "     " << c.name << "& operator=(" << c.name << "&& move)";
  if (c.m_asign)
    out << " {\n\n     return *this;\n     }\n";
  else if (c.del_move)
    out << " = delete;\n";
  else
    out << " = default;\n";
}

void write_ctors(std::ostream &out, const ClassDef &c) {
  out << "     " << c.name;
  if (c.ctor)
    out << c.ctor_str << " {\n\n     }\n";

  else
    out << "() = default;\n";

  out << "     " << c.name << "(const " << c.name << "& copy)";
  if (c.dcctor)
    out << " {\n\n     }\n";
  else if (c.del_copy)
    out << " = delete;\n";
  else
    out << " = default;\n";

  out << "     " << c.name << "(" << c.name << "&& move)";
  if (c.dmctor)
    out << " {\n\n     }\n";
  else if (c.del_move)
    out << " = delete;\n";
  else
    out << " = default;\n";
}

void write_dtor(std::ostream &out, const ClassDef &c) {
  out << "     ~" << c.name << "()";
  if (c.dtor)
    out << " {\n\n     }\n";
  else
    out << " = default;\n";
}

void write_class(std::ostream &out, const ClassDef &c) {
  if (c.name.length()) {

    out << "class " << c.name << " {\n";

    out << "     public:\n";

    write_ctors(out, c);
    write_dtor(out, c);
    write_asign_operator(out, c);

    out << "\n";
    // end of public

    out << "     protected:\n";
    out << "     private:\n";
    out << "};\n\n";
  }
}

int main(int argc, char **argv) {
  auto args = tools::Arg::parse(argc, argv);

  FileDef file;

  for (const auto &a : *args.get()) {

    if (a.first == "-v") {

      cout << "Code generator.\n";
      cout << "Arguments:\n";
      cout << *args.get() << std::endl;
    }

    // Read file name
    else if (a.first == "--name") {

      locale loc;
      if (!file.path.length())
        file.path = a.second + ".h";
      file.name = tools::Path::trim(file.path);

      // Mold class name
      file.cdef.name = a.second;
      file.cdef.name[0] = std::toupper(file.cdef.name[0], loc);

      // Mold header guard
      for (std::string::size_type i = 0; i < file.name.length(); ++i)
        file.header_guard += std::toupper(file.name[i], loc);
      file.header_guard += "_HEADER";
    }

    else if (a.first == "--file") {

      file.path = a.second;
    }

    // Check for a license, read it
    else if (a.first == "--license") {

      string line;
      ifstream license(a.second);

      if (license.is_open()) {
        file.license = "/*\n";
        while (getline(license, line)) {

          file.license += "    " + line + '\n';
        }

        file.license += "*/\n";
        license.close();
      } else
        cout << "Unable to open " << a.second << '\n';
    }

    else if (a.first == "--insert-license") {

      file.insert_license = true;
    }

    else if (a.first == "--ctor") {

      if (a.second != "default") {
        file.cdef.ctor = true;
        if (!a.second.length())
          file.cdef.ctor_str = "()";
        else
          file.cdef.ctor_str = a.second;
      }
    }

    else if (a.first == "--dtor") {

      file.cdef.dtor = true;
    }

    else if (a.first == "--delete") {

      if (a.second == "copy")
        file.cdef.del_copy = true;
      else if (a.second == "move")
        file.cdef.del_move = true;
    }
  }

  if (file.name.length()) {

    // Rename file maybe? May not always be a header in future

    // Check for license insert mode
    if (file.insert_license) {
      fstream f;
      f.open(file.path);
      if (f.is_open()) {

        std::string line, lines;

        while (std::getline(f, line))
          lines += line + '\n';

        f.close();
        f.open(file.path);
        insert_license(f, file.license);
        f << lines;
        f.close();
      } else
        cout << "Could not open: " << file.path << std::endl;
      return 0;
    }

    ofstream header;
    header.open(std::string(file.path), ios::in | ios::trunc);

    if (header.is_open()) {

      insert_license(header, file.license);

      header << "#ifndef " << file.header_guard << '\n';
      header << "#define " << file.header_guard << "\n\n";

      write_class(header, file.cdef);

      header << "#endif//" << file.header_guard << '\n';

      header.close();
    }
  }

  return 0;
}

