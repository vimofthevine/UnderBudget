package com.vimofthevine.underbudget.auth

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import io.ktor.application.*
import io.ktor.locations.*
import io.ktor.request.*
import io.ktor.response.*
import io.ktor.routing.*

import org.slf4j.LoggerFactory

@Location("/users")
class UsersEndpoint()

@Location("/users/{user}")
data class UserEndpoint(val user: UUID)

@Location("/login")
class LoginEndpoint()

data class UserInput(val name: String, val email: String, val password: String)

data class RegistrationResponse(val error: String? = null, val userId: UUID? = null)

fun Routing.auth(db: DbService, passwords: Passwords) {
	val logger = LoggerFactory.getLogger("underbudget.auth")

    post<UsersEndpoint> {
        val user = call.receive<UserInput>()
        val response = db.transaction {
            if (user.name.length < 6) {
                RegistrationResponse(error = "Username must be at least 6 characters in length")
            } else if (user.name.length > 128) {
                RegistrationResponse(error = "Username must be less than 128 characters in length")
            } else if (!user.name.matches("[a-zA-Z0-9._]+".toRegex())) {
                RegistrationResponse(error = "Username must contain only letters, numbers, dots, or underscores")
            } else if (user.password.length < 12) {
                RegistrationResponse(error = "Password must be at least 12 characters in length")
            } else if (db.findUserByName(user.name) != null) {
                RegistrationResponse(error = "Username is already in use")
            } else if (db.findUserByEmail(user.email) != null) {
                RegistrationResponse(error = "User with given email address already exists")
            } else {
                val salt = passwords.generateSalt()
                val hash = passwords.hash(user.password, salt)
                logger.info("length of salt is ${salt.length} and password is ${hash.length}")
                try {
                    val userId = db.createUser(user.name, user.email, salt, hash)
                    RegistrationResponse(userId = userId)
                } catch (e: Throwable) {
                    logger.error("Failed to register user", e)
                    RegistrationResponse(error = "Unable to register user")
                }
            }
        }
        call.respond(response)
    }
    put<UserEndpoint> {
        // it.user
    }
}