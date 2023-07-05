from cryptography.fernet import Fernet
import hashlib
import base64


def generate_key(password: str) -> bytes:
    return base64.b64encode(hashlib.sha512(password.encode("utf-8")).hexdigest()[0:32].encode("utf-8"))


def encrypt(message: bytes, key: bytes) -> bytes:
    return Fernet(key).encrypt(message)


def decrypt(token: bytes, key: bytes) -> bytes:
    return Fernet(key).decrypt(token)
