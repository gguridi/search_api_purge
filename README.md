CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers
 
INTRODUCTION
------------
 
This module provides an integration for Search API to easily remove those 
entities that remain in the search indexes even after having been deleted 
from the Search PI index. Synchronizing databases, custom scripts, etc. can 
often create this kind of situations, and not always it's possible to delete 
the full index and reindex everything again. This module works with the 
entities indexed by Search API, and any kind of search engine plugged in 
on that module.
 
REQUIREMENTS
------------
 
This module requires the following modules to integrate with:
 * [Search API](https://www.drupal.org/project/search_api "Search API")
 
INSTALLATION
------------

This module requires a normal module installation.
 
 * See: [Drupal Module Documentation](
 https://drupal.org/documentation/install/modules-themes/modules-7
 "Drupal 7 modules installation guide") for further information.
 
CONFIGURATION
-------------

The module has no menu or modifiable settings in the UI. 
Set the variable search_api_purge_batch to be able to modify the default
batch size, but it's always possible to edit it before running the purge.
 
MAINTAINERS
-----------

Current maintainers:
 * Gorka Guridi (gguridi) - https://www.drupal.org/u/gorka-guridi

