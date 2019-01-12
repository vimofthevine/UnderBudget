package com.vimofthevine.underbudget.auth

import io.ktor.locations.*

import java.util.UUID

@Location("/users")
class UserResources()

@Location("/users/{userId}")
data class UserResource(val userId: UUID)

@Location("/tokens")
class TokenResources()

@Location("/tokens/{jwtId}")
class TokenResource(val jwtId: String)