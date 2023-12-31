
<H1>Placement System Plugin for Unreal Engine (C++ Code Plugin)</H1>

<a href= "https://youtu.be/I7V5zLQfhk8"> Watch tutorial on youtube here !<a/>

A code plugin that allows very basic city building  functionallities.

<h2>Contains:</h2>
<ul>
    <li> <b> PlacementManagerComponent</b> : A component that allows comunication to the system (e.g placing objects, replacing and checking for selection) </li>
    <li> <b>PlacmentActor </b>: an actor that can be spawned to the scene by PlacementManagerComponent  </li>
    <li> <b>GridManager</b> : an Actor that is responsible for grid and reserve system (so two objects can not be plcaed at the same place at the same time)</li>
</ul>



--------------------------------------------------------------


<H2>PlacementManagerComponent</H2>
the main component that can be added to the player and it is the main way to comunicate with the system.

<h3>Placment</h3>
the process of adding an object to the scene,
proccess simplified into 3 steps (functions):
<ul>
    <li> <b>Placement_Start()</b> :  starts the placement process takes in an actor (PlacementActor).</li>
    <li> <b>Placement_Accept() </b>:  places the actor that been taken in Placement start and places in the currrent location and finishes the placement process.   </li>
    <li> <b>Placement_Cancel()</b> : cancels the placement process.</li>
</ul>

<h3>Replacment</h3>
the process of re-positioning an object that already spawned to the scene
proccess simplified into 3 steps (functions):
<ul>
    <li> <b>Replacement_Start()</b> :  starts the Replacement process takes in an actor (PlacementActor) and starts to replace that actor.</li>
    <li> <b>Replacement_Accept() </b>:  places the actor that been taken in Replacement start and places in the currrent location and finishes the Replacement process.    </li>
    <li> <b> places the actor that been taken in Replacement start to its original locatio and cancels the Replacement proces.</li>
</ul>

<h3>Select</h3>
Act of Selecting the actor under the cursor :

<ul>
    <li> <b>SelectUnderCursor()</b> :  is a function that selects actor under the cursor and calls OnSelected for the actor, and unselects the already selceted ons (calls OnUnselected for that actor).</li>
</ul>


-------------------------------------------------------
<h2>PlacementActor</h2>

An actor that can be placed to the scene by PlacementManagerComponent.

<h3>Functions</h3>
<ul>
    <li> <b> RegisterForReplacement()</b> </li>
    <li> <b>RemoveBuilding() </b></li>

</ul>

<h3>Calllback Functions</h3>
<ul>
    <li> <b> OnPreviewPlacement()</b> : called when the actor is spawned to the scene(called on placment_start for the actor) and not yet accepted (placment_accept() not yet called for the actor </li>
    <li> <b> OnPlaced() </b> : called when the actor spawned (called for actor when Placement_accept called</li>
    <li> <b> OnHover() </b> : Called when the actor hovered by the mouse </li>
    <li> <b> OnUnhover() </b> : Called when the actor unhovered</li>
    <li> <b> OnSelected() </b> : called when the the actor selected</li>
    <li> <b> OnUnselected() </b> : called when this actor is un selected </li>
    <li> <b> OnPlacementPorcessValidation() </b> : called on tick() for validating the current location in both Placement and replacement processes</li>
    <li> <b> OnReplacement_Start() </b> : called when replacement process started for this actor </li>
    <li> <b> OnReplaced() </b> : called when the replacement process is finished for this actor (called for both Replacement_accept and Replacement_cancel )</li>

</ul>


-----------------------------------------------
<h2>GridManager</h2>

An Actor that is responsible for grid and reserving places to actors so two actors can not be positioned at the same place at the same time (one cells can not be reserved more than one PlacementActor)

<h3>Functions</h3>
<ul>
    <li> <b> DrawCells()</b> : draws cells </li>
</ul>


