import os
from glob import glob
from setuptools import setup

package_name = 'tutoria4_bringup'

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
    description='Lança o sistema da Tutoria 4: tutoria4_node_1 e tutoria4_node_2 juntos',
    license='TODO: License declaration',
)
