This module provides an integration for Search API to easily remove those 
entities that remain in the search indexes even after having been deleted 
from the Search PI index. Synchronizing databases, custom scripts, etc. can 
often create this kind of situations, and not always it's possible to delete 
the full index and reindex everything again. This module works with the 
entities indexed by Search API, and any kind of search engine plugged in 
on that module.


It's also possible to run the purge from drush, to be able to run it from 
the command line. No integration with drupal cron has been done yet.

Requirements
This module only requires Search API to work.
