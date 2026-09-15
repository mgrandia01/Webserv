*This project has been created as part of the 42 curriculum by arcmarti, mcuenca-, mgrandia.*

# Webserv

## Description

Webserv is a custom HTTP server written in **C++98** as part of the 42 curriculum.

The goal of the project is to understand how an HTTP server works internally, from accepting client connections and parsing HTTP requests to generating responses and serving resources.

The server can be tested using standard web browsers and HTTP clients. Its behavior is configured through a configuration file inspired by the structure of NGINX configuration files.

The project focuses on:

* HTTP request parsing and response generation.
* Non-blocking client/server communication.
* Handling multiple simultaneous connections.
* Serving static websites.
* Supporting HTTP methods such as `GET`, `POST` and `DELETE`.
* File uploads.
* Directory listing and index files.
* HTTP redirections.
* Custom and default error pages.
* Configurable request body size limits.
* Multiple listening addresses and ports.
* Route-based configuration.

The server is designed around a single event loop using an I/O multiplexing mechanism to manage client connections without blocking.

---

## Features

The current implementation supports the main requirements of the mandatory part of the project:

### HTTP

* `GET`
* `POST`
* `DELETE`
* HTTP status codes
* Request headers and body handling
* HTTP response headers and body
* Persistent connections / connection closing
* Request body size limits

### Static content

* Serving HTML files
* Serving CSS files
* Serving JavaScript files
* Serving images and other static resources
* Configurable document roots
* Index files
* Directory listing / autoindex

### File handling

* File uploads through `POST`
* File deletion through `DELETE`
* Configurable upload directories

### Server configuration

The server uses a configuration file inspired by NGINX.

Configuration can define, among other things:

* Listening interfaces and ports
* Routes / locations
* Allowed HTTP methods
* Root directories
* Index files
* Autoindex
* Redirects
* Upload configuration
* Error pages
* Maximum request body size

### Error handling

The server provides appropriate HTTP status codes and default error pages when no custom page is configured.

Custom error pages can also be configured by the user.

---

## Project Structure

A simplified view of the project is:

```text
.
├── Makefile
├── README.md
├── config/
│   └── ...
├── includes/
│   └── ...
├── src/
│   ├── ...
│   └── ...
└── www/
    ├── ...
    └── uploads/
```

The exact organization may evolve as the project develops.

The `www/` directory contains resources used by the server to demonstrate static content, uploads and other HTTP features.

---

## Instructions

The project must be compiled using **C++98** and the required compiler flags.

Build the project with:

```bash
make
```

Other available Makefile commands are:

```bash
make clean
make fclean
make re
```

The executable expects a configuration file:

```bash
./webserv [configuration_file]
```

For example:

```bash
./webserv config/config_file.conf
```
---

## Configuration

The configuration file defines how the server listens for connections and how different URLs are handled.

A configuration can contain server-level settings such as:

```text
listen
error pages
client_max_body_size
```

and route-level settings such as:

```text
methods
root
index
autoindex
return
upload
```

A simplified example of the idea behind the configuration is:

```text
server
{
    listen 8080;

    root ./www;

    client_max_body_size 10M;

    location /
    {
        methods GET POST DELETE;
        index index.html;
        autoindex off;
    }
}
```

The exact syntax depends on the configuration parser implemented by the project.

## Resources

The following resources were consulted during the development and testing of the project:

* [Webserv — Alim Naqvi](https://www.alimnaqvi.com/blog/webserv) — General information and practical reference for the Webserv project.
* [Understanding NGINX Server and Location Block Selection Algorithms — DigitalOcean](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms) — Reference for NGINX configuration and location handling.
* [RFC 9112 — HTTP/1.1](https://www.rfc-editor.org/info/rfc9112/) — Reference for HTTP/1.1 protocol behavior.
* [HTTP Overview — MDN Web Docs](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Overview) — Reference for HTTP requests, responses, headers and methods.
* **NGINX** and **web browsers** were also used during testing to compare and validate server behavior.

### AI Usage

AI tools were used as a supporting resource during development and learning.

They were mainly used for:

* Understanding HTTP concepts and server behavior.
* Discussing possible approaches to implementing HTTP request and response handling.
* Helping reason about parsing and error-handling cases.
* Suggesting test cases and debugging strategies.
* Reviewing explanations and documentation.

AI-generated suggestions were reviewed, tested and adapted to the actual project requirements and implementation.
