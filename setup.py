from skbuild import setup
from setuptools import find_packages

setup(
    name="quirc",
    version="0.1.0",
    description="Python bindings for quirc, via pybind11",
    author="Maximilian Nöthe",
    author_email="maximilian.noethe@tu-dortmund.de",
    license="MIT",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    cmake_install_dir="src/quirc",
    extras_require={"test": ["pytest"]},
    python_requires=">=3.7",
)
