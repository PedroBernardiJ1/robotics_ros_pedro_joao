import os
from glob import glob
from setuptools import setup

package_name = 'tutoria3_bringup'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
        ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Pedro Bernardi',
    maintainer_email='pedro.bernardi.j@gmail.com',
    description='Lança o sistema da Tutoria 3: tutoria3_node1 e tutoria3_node2 juntos',
    license='Apache-2.0',
)