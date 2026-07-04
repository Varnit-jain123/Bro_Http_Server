# Bro HTTP Server

<div align="center">

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Multithreaded](https://img.shields.io/badge/Multithreaded-Architecture-green?style=for-the-badge)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

**A lightweight, multithreaded C++ web framework built for learning**

[Features](#features) • [Architecture](#architecture) • [Getting Started](#getting-started) • [Documentation](#documentation) • [Contributing](#contributing)

</div>

---

## Overview

Ever wondered what happens behind the scenes when you hit Enter in your browser? Bro HTTP Server pulls back the curtain.

This isn't your typical production-ready framework. It's built specifically for students and curious developers who want to understand how web servers actually work. We've stripped away the magic and complexity of modern frameworks to show you the real mechanics: how HTTP requests are parsed, how sockets communicate, how threads handle concurrent connections, and how it all comes together.

Think of it as your hands-on guide to web server internals. Built from scratch in C++, every line of code is meant to teach something valuable about the web infrastructure we all depend on.

## Features

### What You Get

- **Multithreaded Request Handling** - Each client gets their own thread. Simple, effective, and easy to understand how concurrent connections work.
- **Static File Serving** - Want to serve HTML, CSS, JavaScript? We've got you covered. Just point to your folder and go.
- **RESTful Routing** - Map URLs to functions with GET and POST support. It's as simple as "when someone visits /home, run this code."
- **Query String Parsing** - Those `?name=value` parameters in URLs? Automatically parsed and ready to use.
- **Cookie Management** - Read and write cookies the way real web servers do. Perfect for understanding session management.
- **Request Forwarding** - Chain request handlers together. Process data in steps, like a relay race for HTTP requests.
- **Application-Level Container** - Share data across all your handlers. Think of it as a shared memory space for your application.
- **Startup Services** - Run initialization code when the server starts, with control over execution order.
- **CHTML Template Engine** - Server-side templates that let you inject variables into HTML. Old school, but it works beautifully.
- **Custom Object Serialization** - Turn your C++ objects into strings for HTTP responses with a simple interface.
- **Graceful Shutdown** - Shut down cleanly without leaving connections hanging or resources leaked.

### Why This Matters for Learning

Here's what makes this framework valuable for education:

- You can read and understand every single line of code. No magic, no abstraction layers hiding what's really happening.
- See HTTP requests parsed byte by byte. Understand what `GET /index.html HTTP/1.1` actually means.
- Watch sockets in action. Learn how TCP connections become HTTP conversations.
- Experience thread synchronization firsthand. See what happens when multiple requests hit the same resource.
- Learn design patterns in context, not from abstract examples. See Strategy, Factory, and Observer patterns solving real problems.

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Client Requests                         │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       ▼
         ┌─────────────────────────┐
         │   Socket Listener       │
         │   (Port 6060)           │
         └────────────┬────────────┘
                      │
                      ▼
         ┌─────────────────────────┐
         │   Thread Pool Manager   │
         │   (1 Thread/Request)    │
         └────────────┬────────────┘
                      │
        ┌─────────────┼─────────────┐
        │             │             │
        ▼             ▼             ▼
   ┌────────┐   ┌────────┐   ┌────────┐
   │Worker  │   │Worker  │   │Worker  │
   │Thread 1│   │Thread 2│   │Thread N│
   └────┬───┘   └────┬───┘   └────┬───┘
        │            │            │
        └────────────┼────────────┘
                     │
                     ▼
        ┌─────────────────────────┐
        │   HTTP Request Parser   │
        └────────────┬────────────┘
                     │
                     ▼
        ┌─────────────────────────┐
        │   Route Dispatcher      │
        │   (URL → Handler Map)   │
        └────────────┬────────────┘
                     │
        ┌────────────┼────────────┐
        │            │            │
        ▼            ▼            ▼
   ┌─────────┐ ┌─────────┐ ┌─────────┐
   │ Static  │ │ Dynamic │ │ CHTML   │
   │ Handler │ │ Handler │ │Template │
   └────┬────┘ └────┬────┘ └────┬────┘
        │           │           │
        └───────────┼───────────┘
                    │
                    ▼
        ┌─────────────────────────┐
        │   HTTP Response Builder │
        └────────────┬────────────┘
                     │
                     ▼
        ┌─────────────────────────┐
        │   Send to Client        │
        └─────────────────────────┘
```

## Project Structure


```text
Bro_Http_Server/                # Root project directory
├── CMakeLists.txt              # Main CMake build configuration
├── README.md                   # This file
├── docs/                       # Learning materials and guides
│   └── Learn/
├── include/                    # Public headers
│   └── bro/                    # Framework headers (e.g. bro.h, bro_request.h)
├── src/                        # Core framework implementation
│   ├── CMakeLists.txt          # Builds the 'bro' static library
│   ├── bro.cpp
│   └── [other .cpp files]
├── examples/                   # Example applications
│   ├── bobby/                  # Basic server example
│   │   ├── CMakeLists.txt
│   │   ├── bobby.cpp
│   │   ├── bro-data/           # MIME types
│   │   ├── data/
│   │   ├── vmd_files/
│   │   └── Whatever/           # Static resources
│   │
│   └── varnit/                 # Advanced EduBase example (MySQL required)
│       ├── CMakeLists.txt
│       ├── varnit.cpp
│       ├── db_setup.sql
│       └── static/
├── tests/                      # Unit tests and extra examples
└── scripts/                    # Utility scripts
    └── shutdown/
        └── shutdown_bro.cpp
```

## Getting Started

### What You'll Need

Before diving in, make sure you have:

- **A C++ compiler**: g++ with C++11 or newer works great
- **Your OS**: Works on Linux (Ubuntu/Debian are easiest), or Windows with MinGW
- **Build tools**: GNU Make if you want to get fancy (but it's optional)
- **pthread library**: For multithreading (usually comes with your compiler)

### Building the Server

Let's get this thing running! We use **CMake** to make building incredibly simple across all operating systems.

**1. Grab the code**
   ```bash
   git clone https://github.com/Varnit-jain123/Bro_Http_Server.git
   cd Bro_Http_Server
   ```

**2. Build everything**
   Using CMake, you can build the framework and the example applications in just two commands:
   ```bash
   # Configure the project (creates the 'build' directory)
   cmake -B build

   # Compile the project
   cmake --build build
   ```

That's it! CMake automatically handles compiling the `bro` framework library and linking it to the `bobby` example. You'll find your compiled executables inside the `build/` folder.

---

### 🚀 For Users: Create and Run Your Own Project

Already have the library built? Here's how to write and run your **own** application using BroServer in just 3 steps:

**Step 1: Write your app (e.g. `myapp.cpp`)**
Create a new file in a project folder:
   ```cpp
   #include <bro.h>

   int main() {
       try {
           Bro bro;
           bro.setStaticResourceFolder("static");

           bro.get("/hello", [](Request &request, Response &response) {
               response.setContentType("text/html");
               response << "<h1>Hello from my Bro app!</h1>";
           });

           bro.listen(6060, [](Error &error) {
               if (!error.hasError())
                   cout << "Server is running on port 6060\n";
               else
                   cout << "Error: " << error.getError() << endl;
           });
       } catch (string exception) {
           cout << exception << endl;
       }
       return 0;
   }
   ```

**Step 2: Create a `CMakeLists.txt` for your app**
   ```cmake
   cmake_minimum_required(VERSION 3.18)
   project(MyApp)

   add_executable(myapp myapp.cpp)

   # Link against the Bro HTTP Server framework
   # (assuming you added your app as a subdirectory in the root CMakeLists.txt)
   target_link_libraries(myapp PRIVATE bro)
   ```

**Step 3: Compile and run**
Run `cmake -B build` and `cmake --build build` from the root, and execute your compiled application!

   Then open your browser and visit: `http://localhost:6060/hello`

> 💡 **Tip:** Make sure `bro-data/mime.types` is present in your project folder (you can copy it from `bobby/bro-data/`). The server needs it to detect file types when serving static content.

---

### 🎓 Advanced Example: EduBase (MySQL Integration & CHTML)

The repository also includes `varnit`, a fully featured Student & Faculty Management System built using the Bro framework. It demonstrates advanced capabilities:

- **MySQL Integration**: Direct database connection and querying from C++.
- **REST APIs**: `GET` and `POST` endpoints for CRUD operations.
- **Dynamic CHTML Pages**: Live server rendering with injected variables (e.g. `/report`).
- **Global Application State**: Using `ApplicationLevelContainer` to track total server visits across all users.

To run the EduBase example:
1. Ensure MySQL Server 8.0 is installed and your database is configured.
2. Run `mysql -u root -p < examples/varnit/db_setup.sql` to initialize the database.
3. Open the root `CMakeLists.txt` and uncomment `add_subdirectory(examples/varnit)`.
4. Run `cmake -B build` and `cmake --build build`.
5. Run the server: `cd examples/varnit` then run `..\..\build\examples\varnit\varnit.exe`
6. Visit `http://localhost:6060/index.html`.

---

### Running Your Server

Time to see it in action! **It's important to run the example from its own directory** so it can find its static resource folders (like `Whatever` or `bro-data`).

**1. Fire it up**
   ```bash
   cd examples/bobby
   
   # On Linux/Mac
   ../../build/examples/bobby/bobby
   
   # On Windows
   ..\..\build\examples\bobby\bobby.exe
   ```

**2. You should see something like this:**
   ```
   ---------------------------------------------
   some great function that get call on startup
   The priority number set for this function is 1
   ---------------------------------------------
   ---------------------------------------------
   some great too function that get call on startup
   The priority number set for this function is 1
   ---------------------------------------------
   ---------------------------------------------
   some cool function that get call on startup
   The priority number set for this function is 2
   ---------------------------------------------
   ---------------------------------------------
   some too cool function that get call on startup
   The priority number set for this function is 3
   ---------------------------------------------
   Bro HTTP Server is listening on port 6060
   ```

**3. Open your browser and visit**
   ```
   http://localhost:6060/index.html
   ```

Congratulations! You're now running your own web server.

### Stopping the Server

You have two ways to gracefully shut down the server:

**Option 1: Using the Shutdown Utility (Recommended)**

The server comes with a dedicated shutdown utility located in `scripts/shutdown/`. You can compile it easily:

```bash
cd scripts/shutdown

# On Linux
g++ shutdown_bro.cpp -o shutdown_bro.out

# On Windows (MinGW)
g++ shutdown_bro.cpp -o shutdown_bro.exe -lws2_32 -static
```

Then run it while the server is running:

```bash
# On Linux
./shutdown_bro.out

# On Windows
.\shutdown_bro.exe
```

This sends a special `/shutdown` request that triggers graceful shutdown. The server will:
1. Stop accepting new visitors
2. Finish up with anyone currently connected
3. Clean up all resources properly
4. Shut down worker threads gracefully
5. Release everything cleanly

**Option 2: Manual Interrupt**

- **On Linux/Mac**: Hit `Ctrl+C` in your terminal
- **On Windows**: Press `Ctrl+C` or close the window

Both methods ensure a proper shutdown - no crashed connections, no leaked resources, no orphaned threads!

## Documentation

### Example Endpoints

Here's what the example application (`bobby.cpp`) gives you to play with:

| Endpoint | Method | What It Does | What You Need |
|----------|--------|-------------|------------|
| `/index.html` | GET | Your homepage - start here! | Nothing, just visit it |
| `/save_test1_data` | GET | Shows how query strings work | Try: `?nm=John&ct=Boston` |
| `/save_test2_data` | POST | Demonstrates form handling and cookies | Form data |
| `/firstCartoonFilm` | GET | Stores data in the app container | None |
| `/secondCartoonFilm` | GET | Retrieves that stored data | None |
| `/coolOne` | GET | First step in a forwarding chain | None |
| `/Slogan` | GET | Reads a file and displays it | None |
| `/SloganOfTheDay` | GET | Shows off the CHTML template engine | None |
| `/aRequest` | GET | Passes data to the next handler | None |
| `/bRequest` | GET | Receives that data and sets cookies | None |

### How to Use It - Real Examples

Let's walk through some actual code you can write:

#### 1. Handling a Simple GET Request

This is the bread and butter of web servers:

```cpp
bro.get("/save_test1_data", [](Request &request, Response &response) {
    // Grab parameters from the URL
    string name = request["nm"];      // Gets 'nm' from ?nm=John&ct=Boston
    string city = request["ct"];      // Gets 'ct' from the same URL
    
    // Send back some HTML
    response.setContentType("text/html");
    response << "<h1>Thanks, " << name << " from " << city << "!</h1>";
});
```

Visit: `http://localhost:6060/save_test1_data?nm=John&ct=Boston`

#### 2. Working with Cookies

Cookies are how websites remember you. Here's how to use them:

```cpp
bro.post("/save_test2_data", [](Request &request, Response &response) {
    // Read cookies that the browser sent
    string rollNo = request.getCookieValue("RollNumber");
    string name = request.getCookieValue("Name");
    
    // Do something with them
    cout << "Student: " << name << " (Roll: " << rollNo << ")" << endl;
    
    response.setContentType("text/html");
    response << "<b>Welcome back, " << name << "!</b>";
});
```

#### 3. Sharing Data Across Your Application

Sometimes you need all handlers to access the same data:

```cpp
bro.get("/firstCartoonFilm", [](Request &request, Response &response, 
                                ApplicationLevelContainer &container) {
    // Store something that any handler can access
    string *filmName = new string("The Jungle Book");
    container.set("firstFilm", filmName, NULL, NULL);
    
    response.setContentType("text/html");
    response << "<h1>Stored: The Jungle Book</h1>";
    response << "<a href='/secondCartoonFilm'>Check what we stored</a>";
});
```

Then retrieve it later:

```cpp
bro.get("/secondCartoonFilm", [](Request &request, Response &response,
                                 ApplicationLevelContainer &container) {
    // Get the data we stored earlier
    string *film;
    container.get("firstFilm", &film, NULL, NULL);
    
    response.setContentType("text/html");
    response << "<h1>We stored: " << *film << "</h1>";
});
```

#### 4. Forwarding Requests (The Relay Race)

Sometimes you want to process a request in multiple steps:

```cpp
bro.get("/coolOne", [](Request &request, Response &response) {
    // Do some work here
    cout << "First handler: preparing the data..." << endl;
    
    // Pass it to the next handler
    _forward_(request, string("/coolTwo"));
});

bro.get("/coolTwo", [](Request &request, Response &response) {
    // Do more work
    cout << "Second handler: processing further..." << endl;
    
    // Keep the chain going
    _forward_(request, string("/coolThree"));
});
```

#### 5. Template Variables (CHTML)

Want to inject data into HTML templates? Here you go:

```cpp
bro.get("/SloganOfTheDay", [](Request &request, Response &response) {
    // Set simple variables
    request.setCHTMLVariable("city1", "Indore");
    request.setCHTMLVariable("city2", "Goa");
    
    // Set numeric variables
    int temperature = 35;
    request.setCHTMLVariable("temp", temperature);
    
    // Even set custom objects!
    Bulb bulb;
    bulb.setWattage(100);
    bulb.setPrice(250);
    request.setCHTMLVariable("bulb", &bulb);
    
    // Forward to the template
    _forward_(request, string("/WordsOfWisdom.chtml"));   
});
```

#### 6. Initialize on Startup

Need to set things up when the server starts? Use startup services:

```cpp
// This runs first (priority 1 = highest)
bro.addStartupService(1, []() {
    cout << "Connecting to database..." << endl;
    // Initialize your database connection
});

// This runs second (priority 2)
bro.addStartupService(2, [](ApplicationLevelContainer &container) {
    cout << "Loading configuration..." << endl;
    // Load config and store it in the container
});

// Lower numbers = higher priority
```

### Making Your Own Serializable Objects

Want to pass your own objects to templates? Implement `Stringifyable`:

```cpp
class Bulb : public Stringifyable {
    private:
        int wattage;
        int price;
    
    public:
        // Your normal getters and setters
        void setWattage(int w) { this->wattage = w; }
        int getWattage() { return this->wattage; }
        void setPrice(int p) { this->price = p; }
        int getPrice() { return this->price; }
        
        // This is how it becomes a string
        string stringify() {
            return "A " + to_string(wattage) + "W bulb for ₹" + to_string(price);
        }
};
```

Now you can use it anywhere:

```cpp
Bulb myBulb;
myBulb.setWattage(60);
myBulb.setPrice(150);

request.setCHTMLVariable("bulb", &myBulb);
// Template will see: "A 60W bulb for ₹150"
```

## Cool Tricks and Advanced Stuff

### Passing Data Between Handlers

Ever wanted to process a request in stages? Like "first check the user, then get their data, then format it"? Here's how:

```cpp
bro.get("/aRequest", [](Request &request, Response &response) {
    // Generate some data (like a user's score)
    int score = 100;
    
    // Attach it to the request so the next handler can use it
    request.set("score", score, NULL, NULL);
    
    cout << "Passing score to next handler..." << endl;
    _forward_(request, string("/bRequest"));
});

bro.get("/bRequest", [](Request &request, Response &response) {
    // Receive the score from the previous handler
    int receivedScore;
    request.get("score", &receivedScore, NULL, NULL);

    cout << "Got score: " << receivedScore << endl;
    response.setContentType("text/html");
    response << "<h1>Your score: " << receivedScore << "</h1>";
});
```

This is super useful for building modular, reusable request handlers!

### Working with Cookies Like a Pro

Cookies are how websites remember who you are. Here's the full picture:

```cpp
bro.get("/login", [](Request &request, Response &response) {
    // Create cookies (these get sent to the browser)
    Cookie rollNumber("RollNumber", "101");
    Cookie userName("Name", "Bobby");
    Cookie userCity("City", "Indore");
    
    // Multiple ways to add them to the response
    response << rollNumber << userName;     // Stream them in
    response.addCookie(userCity);           // Or add explicitly
    
    response.setContentType("text/html");
    response << "<h1>Welcome! You're logged in.</h1>";
});
```

Now when the user comes back, you can read those cookies:

```cpp
bro.get("/dashboard", [](Request &request, Response &response) {
    // Browser automatically sends cookies back
    string name = request.getCookieValue("Name");
    string city = request.getCookieValue("City");
    
    response << "<h1>Welcome back, " << name << " from " << city << "!</h1>";
});
```

### Setting Up Your Static Files

Got CSS, JavaScript, images? Point Bro to your folder:

```cpp
bro.setStaticResourceFolder("Whatever");
```

Now all your static files in the "Whatever" folder are automatically served. Easy!

## Performance & Limitations

Let's be real about what this server can and can't do:

**The Good:**
- **Simple and predictable**: One thread per request means easy debugging
- **Low latency**: Static files are served fast, dynamic routes depend on your code
- **Stable under moderate load**: Works great for development, testing, and learning

**The Reality Check:**
- **Not for production**: This is a teaching tool, not the next nginx
- **Thread limit**: Designed for dozens to a few hundred concurrent connections, not thousands
- **Memory scales linearly**: Each connection uses memory. More connections = more RAM
- **No fancy optimizations**: We chose clarity over clever optimizations

**Best used for:**
- Learning how web servers work
- Building student projects and demos  
- Understanding HTTP at a fundamental level
- Prototyping ideas before scaling up

**Not recommended for:**
- Production websites with real traffic
- High-performance APIs
- Anything mission-critical

Think of it as your training wheels for web development. Once you understand the concepts here, you'll appreciate production frameworks so much more!

## What's Next?

This project is a living learning tool. Here's what we're thinking about adding:

**Performance & Scalability:**
- [ ] Real thread pool (instead of spawning threads for each request)
- [ ] HTTP support with multiplexing
- [ ] Connection pooling and keep-alive improvements

**Security:**
- [ ] SSL/TLS support (HTTPS)
- [ ] Rate limiting to prevent abuse
- [ ] Basic DDoS protection
- [ ] Input validation helpers

**Features:**
- [ ] WebSocket support for real-time apps
- [ ] Middleware pipeline (like Express.js)
- [ ] JSON parsing and generation helpers
- [ ] Session management framework
- [ ] File upload handling (multipart/form-data)
- [ ] Template engine improvements (loops, if statements)

**Developer Experience:**
- [ ] Better error messages
- [ ] Logging system
- [ ] Hot reload during development
- [ ] Built-in debugging tools

**Cool Stuff:**
- [ ] Database connection pooling examples
- [ ] Virtual host support (multiple sites, one server)
- [ ] URL rewriting
- [ ] Response compression (gzip/brotli)
- [ ] CORS configuration helpers

Got ideas? We're all ears! This is meant to evolve based on what people actually want to learn.

## Contributing

We'd love your help making this better! Whether you're fixing a bug, adding a feature, or improving documentation, all contributions are welcome.

### How to Jump In

1. **Fork this repo** - Click that Fork button up top
2. **Create your feature branch**  
   ```bash
   git checkout -b feature/something-awesome
   ```
3. **Make your changes** - Write some code!
4. **Commit with a clear message**  
   ```bash
   git commit -m 'Add feature: something awesome'
   ```
5. **Push to your fork**  
   ```bash
   git push origin feature/something-awesome
   ```
6. **Open a Pull Request** - Tell us what you built!

### What We Care About

**Code Quality:**
- Keep it readable. Clarity beats cleverness every time.
- Add comments explaining the "why", especially around HTTP protocol details.
- If you add a feature, show us how to use it with an example.

**Documentation:**
- Update the README if you change behavior
- Add code comments for complex logic  
- Write educational commit messages (remember, people learn from these!)

**Style:**
- Follow the existing code style
- Variable names should be clear and descriptive
- Keep functions focused and not too long

### Great First Contributions

Looking for ideas? Try these:

- Add more example endpoints to `bobby.cpp`
- Improve error messages to be more helpful
- Write tutorials or guides
- Add unit tests
- Fix bugs (check the Issues tab)
- Improve code comments

### Not Sure Where to Start?

Open an issue and ask! We're happy to help you find something that matches your interests and skill level.

Remember: This is a learning project. "Beginner-friendly" contributions are just as valuable as advanced ones. We all started somewhere!

## Need Help?

Stuck? Confused? Found a bug?

- **Bug reports & feature ideas**: [Open an issue](https://github.com/Varnit-jain123/Bro_Http_Server/issues)
- **Email**: varnitjaintj@gmail.com

Don't be shy! We're here to help you learn.

---

<div align="center">

**Bro HTTP Server — designed and engineered by Varnit Jain for those who dare to build from scratch.**

[⬆ Back to top](#bro-http-server)

</div>