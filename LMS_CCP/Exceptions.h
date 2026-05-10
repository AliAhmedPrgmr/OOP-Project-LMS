#pragma once

#include <stdexcept>
#include <string>

/**
 * Base class for all Learning Management System (LMS) exceptions.
 * Derived from std::runtime_error for standard compatibility.
 */
class LMSException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/**
 * Thrown when user provides invalid inputs (e.g., empty names, negative IDs, CGPA > 4.0).
 */
class InvalidInputException : public LMSException {
public:
    explicit InvalidInputException(const std::string& message)
        : LMSException(message) {}
};

/**
 * Thrown when trying to add an entity that already exists (e.g., duplicate Student ID or Course).
 */
class DuplicateEntityException : public LMSException {
public:
    explicit DuplicateEntityException(const std::string& message)
        : LMSException(message) {}
};

/**
 * Thrown when a requested resource is not found (e.g., searching for a non-existent ID).
 */
class NotFoundException : public LMSException {
public:
    explicit NotFoundException(const std::string& message)
        : LMSException(message) {}
};

/**
 * Thrown for illegal operations (e.g., performing a task without proper permissions).
 */
class InvalidOperationException : public LMSException {
public:
    explicit InvalidOperationException(const std::string& message)
        : LMSException(message) {}
};