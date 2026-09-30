from setuptools import find_packages, setup

package_name = 'physics_operations'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='host',
    maintainer_email='pedro.bernardi.j@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'vector_distance_server_node = physics_operations.vector_distance_server_node:main',
            'vector_distance_cliente_node = physics_operations.vector_distance_cliente_node:main',
        ],
    },
)
