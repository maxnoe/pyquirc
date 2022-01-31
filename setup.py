from skbuild import setup
from setuptools import find_packages

setup(
    packages=find_packages(where='src'),
    package_dir={'': 'src'},
    cmake_install_dir="src/quirc",
    cmake_args=[
        '-DCPP_EXAMPLE:BOOL=OFF',
    ],
)
