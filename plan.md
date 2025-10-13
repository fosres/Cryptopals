# Plan

After completing CryptoPals in C and Rust I intend to

practice implementing a few cryptographic primitives:

Classic McEliece and ML-KEM.

Public-Key Encryption is the most in danger of quantum computing

attacks.

And safe public key exchange still remains a serious problem in real

life so it is something I should study. We need to talk about managing

our Web of Trust.

It seems the only way to gurantee a safe Web of Trust is to verify

ownership of public keys in person. If that is not an option let 

someone else sign the other's public key. But know that said signer's

public key *must* have been verified in person. So you should not

setup the other public key.

As for your Public Key Encapsulation implementations make sure you

test them using test vectors published by the developers. Test

side channel resistance based on their guidelines.

--------------------------------------------------

Employment:

You probably will get employed more likely for having a library

of battle-tested crypto code than auditing crypto code. But you

will naturally do this in your journey being a competent auditor

anyhow.

---------------------------------------------------

The first projects I will audit are GNUPG and Sequoia-PGP.

Despite its flaws PGP is the only Internet standard protocol

that allows you to choose who you entrust to manage your

information from scratch.

As the colleague pointed out pay attention to key management

issues. Audit ML-KEM encapsulation in GNUPG first.

Before you audit that implement ML-KEM after completing

CryptoPals in C code. Also audit how GNUPG handles *key management*.

Especially key encapsulation keys. The most brittle part of modern

cryptography is safely exchanging public keys--and then encapsulating

secret keys using Key Encapsulation Mechanisms in second order of

importance.

A colleague at work warned me about key mismanagement--that is the

downfall of proper public key encryption.
