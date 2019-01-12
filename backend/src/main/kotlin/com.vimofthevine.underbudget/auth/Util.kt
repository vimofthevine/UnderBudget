package com.vimofthevine.underbudget.auth

import io.ktor.application.ApplicationCall
import io.ktor.auth.authentication
import io.ktor.auth.jwt.JWTPrincipal

import java.util.UUID

val ApplicationCall.userId get() =
	authentication.principal<JWTPrincipal>()?.payload?.getClaim("userId")?.asString()?.let {
        UUID.fromString(it)
    }