package com.vimofthevine.underbudget.auth

import java.util.Date
import java.util.UUID

/** Model of a user to be used within the backend. */
data class User(val id: UUID?, val name: String, val email: String,
               @Transient val hashedPassword: String, @Transient val salt: String)

/** User-supplied information about themselves. */
data class UserData(val name: String, val email: String, val password: String)

data class Token(val id: UUID?, val jwtId: String, @Transient val userId: UUID,
                 val issued: Date, val subject: String)

data class Tokens(val tokens: List<Token>)

data class RegistrationResponse(val error: String? = null, val userId: UUID? = null)

data class LoginResponse(val error: String? = null, val token: String? = null)