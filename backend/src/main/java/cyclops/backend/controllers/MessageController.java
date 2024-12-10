package cyclops.backend.controllers;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import cyclops.backend.models.Message;
import cyclops.backend.services.MessageService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

@RestController
@RequestMapping("/message")
@Tag(name = "Message Controller", description = "Handles operations related to messages")
public class MessageController {

    private final MessageService messageService;

    public MessageController(MessageService messageService) {
        this.messageService = messageService;
    }

    @PostMapping
    @Operation(summary = "Save a new message", description = "Stores a new message in the database.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Message saved successfully"),
            @ApiResponse(responseCode = "400", description = "Invalid request body", content = @Content)
    })
    public void saveMessage(@RequestBody Message message) {
        messageService.saveMessage(message);
    }

    @GetMapping("/{id}")
    @Operation(summary = "Retrieve a message by ID", description = "Fetches a specific message using its unique identifier.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Message retrieved successfully", content = @Content(schema = @Schema(implementation = Message.class))),
            @ApiResponse(responseCode = "404", description = "Message not found", content = @Content)
    })
    public Message getMessage(@PathVariable String id) {
        return messageService.getMessage(id)
                .orElseThrow(
                        () -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Message not found with id: " + id));
    }

    @GetMapping("/last")
    @Operation(summary = "Retrieve the last message", description = "Fetches the most recent message from the database.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Last message retrieved successfully", content = @Content(schema = @Schema(implementation = Message.class))),
            @ApiResponse(responseCode = "204", description = "No new message available", content = @Content)
    })
    public Message getLasMessage() {
        return messageService.getLastMesage()
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NO_CONTENT, "NO NEW MESSAGE"));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Delete a message by ID", description = "Deletes a message using its unique identifier.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Message deleted successfully"),
            @ApiResponse(responseCode = "404", description = "Message not found", content = @Content)
    })
    public void deleteMessage(@PathVariable String id) {
        messageService.deleteMessage(id);
    }

}
